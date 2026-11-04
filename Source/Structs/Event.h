#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

class Listener
{
public:
	Listener() = default;
	explicit Listener(function<void()> cancel) : cancel(move(cancel)) {} 

	Listener(const Listener&) = delete;
	Listener& operator=(const Listener&) = delete;

	Listener(Listener&& other) noexcept : cancel(move(other.cancel)) { other.cancel = nullptr; }
	Listener& operator=(Listener&& other) noexcept
	{
		if (this != &other)
		{
			Unsubscribe();                      
			cancel = std::move(other.cancel);
			other.cancel = nullptr;
		}
		return *this;
	}
	~Listener() { Unsubscribe(); }

	void Unsubscribe()
	{
		if (!cancel) { return; }
		function<void()> cancelNow = move(cancel);
		cancel = nullptr;
		cancelNow();
	}

	bool IsSubscribed() const { return cancel != nullptr; }

private:
	function<void()> cancel;
};

template <typename... Args>
class Event
{
public:
	using Callback = function<void(Args...)>;

	Event() : state(make_shared<State>()) {}
	Event(const Event&) = delete;                
	Event& operator=(const Event&) = delete;

	Listener Subscribe(Callback callback)
	{
		unsigned id = state->nextId++;
		state->entries.push_back(Entry{ id, make_shared<Callback>(move(callback)), true });

		weak_ptr<State> weak = state;
		return Listener([weak, id]() { if (shared_ptr<State> s = weak.lock()) { s->Remove(id); } });
	}

	template <typename T, typename Method>
	Listener Subscribe(T* object, Method method)
	{
		return Subscribe(Callback([object, method](Args... args) { (object->*method)(args...); }));
	}

	
	void Invoke(Args... args)
	{
		shared_ptr<State> keepAlive = state;
		State& s = *keepAlive;

		++s.invoking;
		size_t count = s.entries.size();            
		for (size_t i = 0; i < count; ++i)
		{
			if (!s.entries[i].active) { continue; }
			shared_ptr<Callback> callback = s.entries[i].callback; 
			(*callback)(args...);
		}
		if (--s.invoking == 0 && s.needsCleanup) { s.Cleanup(); }
	}

	size_t SubscriberCount() const
	{
		size_t n = 0;
		for (const Entry& entry : state->entries) { if (entry.active) { ++n; } }
		return n;
	}

	void Clear()                                 
	{
		for (Entry& entry : state->entries) { entry.active = false; entry.callback.reset(); }
		state->needsCleanup = true;
		if (state->invoking == 0) { state->Cleanup(); }
	}

private:
	struct Entry
	{
		unsigned id;
		shared_ptr<Callback> callback;
		bool active;
	};

	struct State
	{
		vector<Entry> entries;
		unsigned nextId = 1;
		int invoking = 0;                        
		bool needsCleanup = false;

		void Remove(unsigned id)
		{
			for (Entry& entry : entries)
			{
				if (entry.id == id && entry.active)
				{
					entry.active = false;
					entry.callback.reset();
					needsCleanup = true;
					break;
				}
			}
			if (invoking == 0 && needsCleanup) { Cleanup(); }   
		}

		void Cleanup()
		{
			entries.erase(remove_if(entries.begin(), entries.end(), [](const Entry& e) { return !e.active; }), entries.end());
			needsCleanup = false;
		}
	};

	shared_ptr<State> state;
};