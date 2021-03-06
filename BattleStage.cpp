#include "BattleStage.h"
#include "LevelComplete.h"
#include "Button.h"
#include "player.h"
#include "Bullet.h"
#include "Enemies.h"
#include "Background.h"

bool BattleStage::open(sf::RenderWindow* window, int difficulty) {
	//prepare level
	bool done = false;
	Button exitB = Button(0,0,50,50,sf::Color(64,64,64,255));

	// keyboard display parts
	//sf::RectangleShape upSq = sf::RectangleShape(sf::Vector2f(50,50));
	//sf::RectangleShape downSq = sf::RectangleShape(sf::Vector2f(50,50));
	//sf::RectangleShape leftSq = sf::RectangleShape(sf::Vector2f(50,50));
	//sf::RectangleShape rightSq = sf::RectangleShape(sf::Vector2f(50,50));
	//sf::RectangleShape zSq = sf::RectangleShape(sf::Vector2f(50,50));
	//sf::RectangleShape xSq = sf::RectangleShape(sf::Vector2f(50,50));
	sf::RectangleShape scoreBar = sf::RectangleShape(sf::Vector2f(300,15));
	//display colors
	//upSq.setFillColor(sf::Color::Green);
	//downSq.setFillColor(sf::Color::Green);
	//leftSq.setFillColor(sf::Color::Green);
	//rightSq.setFillColor(sf::Color::Green);
	//zSq.setFillColor(sf::Color::Red);
	//xSq.setFillColor(sf::Color::Cyan);
	//display positons
	//upSq.setPosition(50,500);
	//downSq.setPosition(50,550);
	//leftSq.setPosition(0,550);
	//rightSq.setPosition(100,550);
	//zSq.setPosition(0,500);
	//xSq.setPosition(100,500);
	scoreBar.setPosition(0,585);
	//list drawables
	drawables = std::list<sf::Drawable*>();
	//drawables.push_front(exitB.getShape());
	drawables.push_front(p1.getSprite());
	drawables.push_front(&scoreBar);

    //set up level
	int enemiesLeft = 2*(5 + 5*difficulty);
	int enemyRate = (difficulty > 6) ? 5 : 35 - 5*difficulty;
	int enemyTimer = 1;
	if (difficulty == 1) p1.score = 300;

	Background bg = Background();
	bg.setGroundColor(sf::Color(0,200,0,255));
	bg.setParticleColor(sf::Color(0,240,0,255));
	bg.setIntensity(difficulty);
	for(int i = 0; i < 600/difficulty; i++) bg.logic();

	//bulk loop
	while (!done) {
		p1.logic();
		p1.score += BulletLogic();
		EnemyLogic();
		bg.logic();

		sf::Event event;
		while(window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {//check for closed window
				done = true;
				window->close();
			}
			if (event.type == sf::Event::MouseButtonPressed) { //if the exit button is clicked, prepare to back out of screen
				if (event.mouseButton.button == sf::Mouse::Left &&
					exitB.pointOnBox(event.mouseButton.x, event.mouseButton.y)) done = true;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					//drawables.push_back(&upSq);
					p1.input(player::UP);
				}
				if (event.key.code == sf::Keyboard::Down) {
					//drawables.push_back(&downSq);
					p1.input(player::DOWN);
				}
				if (event.key.code == sf::Keyboard::Left) {
					//drawables.push_back(&leftSq);
					p1.input(player::LEFT);
				}
				if (event.key.code == sf::Keyboard::Right) {
					//drawables.push_back(&rightSq);
					p1.input(player::RIGHT);
				}
				if (event.key.code == sf::Keyboard::Z) {
                    //drawables.push_back(&zSq);
                    newbullet(p1.getX(),p1.getY());
                    newbullet(p1.getX()+49,p1.getY());
                    //p1.score += 10;
                }
				if (event.key.code == sf::Keyboard::X) {
					//drawables.push_back(&xSq);
					//temporary for enemy testing:
					//newEnemy(rand()%460,0);
					//p1.score -= 10;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Up) {
					//drawables.remove(&upSq);
					p1.input(player::STOP);
				}
				if (event.key.code == sf::Keyboard::Down) {
					//drawables.remove(&downSq);
					p1.input(player::STOP);
				}
				if (event.key.code == sf::Keyboard::Left) {
					//drawables.remove(&leftSq);
					p1.input(player::STOP);
				}
				if (event.key.code == sf::Keyboard::Right) {
					//drawables.remove(&rightSq);
					p1.input(player::STOP);
				}
				if (event.key.code == sf::Keyboard::Z); //drawables.remove(&zSq);
				if (event.key.code == sf::Keyboard::X); //drawables.remove(&xSq);
			}
		}

		//enemy deployment
		enemyTimer = (enemyTimer + 1) % enemyRate;
		if (!enemyTimer) newEnemy(rand()%415,-90), enemiesLeft--;

		//update scoreBar
		scoreBar.setSize(sf::Vector2f(p1.score/2 , 15));
		scoreBar.setFillColor(sf::Color(255 - p1.score*255/1000,p1.score*255/1000,20,255));

        //check win/lose conditions
        if (p1.score >= 1000 || enemiesLeft == 0) {
            DeleteAllBullets();
            DeleteAllEnemys();
            LevelComplete().open(window, p1.score);
            p1.input(player::STOP);
            return (p1.score >= 1000);
        }
        if (p1.score <= 0) {//game over
            p1.score = 0; //temporary filler untill GameOver is implemented
        }

		//draw sequence
		window->clear();
		bg.render(window);
		for (std::list<sf::Drawable*>::iterator it = drawables.begin(); it != drawables.end(); it++)
			window->draw(**it);
        RenderAndDeleteBullet(window);
		RenderAndDeleteEnemy(window);
		window->display();
	}
	DeleteAllBullets();
	return true;
}
