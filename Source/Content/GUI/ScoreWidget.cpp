#include "ScoreWidget.h"

#include "Architecture/GUI/GUI_Text.h"

ScoreWidget::ScoreWidget()
{
	currentScore = new GUI_Text("0", GUITransform(Vector2(105.f, -50.f), 1.f, 1.f, Vector2(1.f), BOTTOM_LEFT, BOTTOM_LEFT), Color::Black());
	highScore = new GUI_Text("0", GUITransform(Vector2(175.f, -5.f), 1.f, 1.f, Vector2(1.f), BOTTOM_LEFT, BOTTOM_LEFT), Color::Black());

	elements.push_back(currentScore);
	elements.push_back(highScore);
	elements.push_back(new GUI_Text("Score:", GUITransform(Vector2(5.f, -50.f), 1.f, 1.f, Vector2(1.f), BOTTOM_LEFT, BOTTOM_LEFT), Color::Black()));
	elements.push_back(new GUI_Text("High Score:", GUITransform(Vector2(5, -5.f), 1.f, 1.f, Vector2(1.f), BOTTOM_LEFT, BOTTOM_LEFT), Color::Black()));
}

void ScoreWidget::SetScore(int score)
{
	currentScore->text = std::to_string(score);
}

void ScoreWidget::SetHighscore(int highscore)
{
	highScore->text = std::to_string(highscore);
}
