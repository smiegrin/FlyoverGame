#pragma once
#include <SFML/Graphics.hpp>

class player
{
private:

	static const int width = 40;
	static const int height = 40;
	int x, y, xVel, yVel, score;
	sf::RectangleShape sprite;
public:
	enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
	bool gameOver;
	player();
	void Draw();
	void input(Direction);
	void setVelocity(int,int);
	void logic();
	sf::RectangleShape* getSprite();
};

