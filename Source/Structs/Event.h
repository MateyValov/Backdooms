#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

// ======================================================================================
//  Event<Args...>: something that happens, which any number of listeners can react to.
//
//  Declare an event type by listing what it carries:
//      Event<>                       carries nothing
//      Event<int>                    one value
//      Event<int, int>               several values
//      Event<Object*, float>         anything: pointers, structs, const std::string&, ...
//
//  The owner fires it, the listeners subscribe. The owner never knows who is listening:
//      class Player { public: Event<int, int> OnHealthChanged;  ... };
//      player->OnHealthChanged.Invoke(health, maxHealth);                         // owner
//      sub = player->OnHealthChanged.Subscribe([this](int h, int m) { ... });     // listener
//      sub = player->OnHealthChanged.Subscribe(this, &PlayerHUD::SetHealthBar);   // or a member function
//
//  Subscribe returns a Subscription. Keep it as a member of the listener: when the listener is
//  destroyed (or you call Unsubscribe / assign a new one) the listener is removed again, so an
//  event can never call into an object that no longer exists.
// ======================================================================================

// One listener's ticket. Move-only. Cancels the subscription when it is destroyed.
// Safe even if the event was destroyed first.
class Listener
{
public:
	Listener() = default;
	explicit Listener(function<void()> cancel) : cancel(move(cancel)) {}   // made by Event::Subscribe

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
	Event(const Event&) = delete;                // copying would silently share the listener list
	Event& operator=(const Event&) = delete;

	// Adds a listener. Listeners are called in the order they subscribed.
	Listener Subscribe(Callback callback)
	{
		unsigned id = state->nextId++;
		state->entries.push_back(Entry{ id, make_shared<Callback>(move(callback)), true });

		weak_ptr<State> weak = state;       // weak: the event may be gone by the time the ticket is cancelled
		return Listener([weak, id]() { if (shared_ptr<State> s = weak.lock()) { s->Remove(id); } });
	}

	// Same, for a member function:  event.Subscribe(this, &PlayerHUD::SetHealthBar)
	template <typename T, typename Method>
	Listener Subscribe(T* object, Method method)
	{
		return Subscribe(Callback([object, method](Args... args) { (object->*method)(args...); }));
	}

	// Calls every listener with these values. Only the owner of the event should call this.
	// Safe if a listener subscribes, unsubscribes (itself or another) or even destroys the
	// event's owner while this runs: new listeners are called from the next Invoke on, and a
	// listener removed during this Invoke is not called after that.
	void Invoke(Args... args)
	{
		shared_ptr<State> keepAlive = state;   // the event's owner may be destroyed by a listener
		State& s = *keepAlive;

		++s.invoking;
		size_t count = s.entries.size();            // listeners added during the loop wait for the next time
		for (size_t i = 0; i < count; ++i)
		{
			if (!s.entries[i].active) { continue; }
			shared_ptr<Callback> callback = s.entries[i].callback;   // keeps it alive even if it unsubscribes itself
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

	void Clear()                                 // removes every listener
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
		int invoking = 0;                        // > 0 while Invoke is running (possibly nested)
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
			if (invoking == 0 && needsCleanup) { Cleanup(); }   // while Invoke runs, the erase waits until it is finished
		}

		void Cleanup()
		{
			entries.erase(remove_if(entries.begin(), entries.end(), [](const Entry& e) { return !e.active; }), entries.end());
			needsCleanup = false;
		}
	};

	shared_ptr<State> state;
};