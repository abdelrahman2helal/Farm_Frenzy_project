#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 4;
	curr_vel.y = 4;
	productCounter = 5;
	maxCounter = 10; 
	
}

void Animal::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

	// Bar background (red)
	pWind->SetPen(RED, 1);
	pWind->SetBrush(RED);
	pWind->DrawRectangle(RefPoint.x, RefPoint.y - 10, RefPoint.x + width, RefPoint.y - 2);

	// Bar fill (green) based on productCounter / maxCounter
	float ratio = (float)productCounter / (float)maxCounter;
	int fillWidth = (int)(width * ratio);
	pWind->SetPen(GREEN, 1);
	pWind->SetBrush(GREEN);
	pWind->DrawRectangle(RefPoint.x, RefPoint.y - 10, RefPoint.x + fillWidth, RefPoint.y - 2);
}

void Animal::incrementCounter()
{
	

	productCounter--;  // decreases every second

	if (productCounter <= 0)
	{
		productCounter = 0;
	}
}


Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
	maxCounter = 10; // 10 seconds
}

void Chick::moveStep()
{
		// 1. Move the animal by adding velocity to its current position
		RefPoint.x += curr_vel.x;
		RefPoint.y += curr_vel.y;

		// 2. Introduce random direction changes (e.g., 2% chance to change direction each step)
		int random_chance = rand() % 100;
		if (random_chance < 2) {
			curr_vel.x = (rand() % 11 - 5); // Random velocity between -2 and 2
			curr_vel.y = (rand() % 11 - 5);
		}

		// 3. Bounce off the walls using the boundaries from GameConfig/BudgetBar
		// If it hits the left or right wall, reverse the X velocity
		if (RefPoint.x < 50 || RefPoint.x > config.windWidth - 50) {
			curr_vel.x = -curr_vel.x;
		}
		// If it hits the top or bottom wall, reverse the Y velocity
		if (RefPoint.y < (config.toolBarHeight * 2) + 50 || RefPoint.y > config.windHeight - config.statusBarHeight - 50) {
			curr_vel.y = -curr_vel.y;
		}
	
	//TO DO: add code for cleanup and game exit here
	/*
	//draw image of this object in the field
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	*/
	cout << "Icon Chick Clicked" << endl;
}

Product* Chick::produceProduct()
{
	point p = { RefPoint.x, RefPoint.y };
	return new Product(pGame, p, 30, 30, "images\\egg.jpg", EGG);
}


Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
	maxCounter = 15; // 15 seconds
}

void Cow::moveStep()
{
		// 1. Move the animal by adding velocity to its current position
		RefPoint.x += curr_vel.x;
		RefPoint.y += curr_vel.y;

		// 2. Introduce random direction changes (e.g., 2% chance to change direction each step)
		int random_chance = rand() % 100;
		if (random_chance < 2) {
			curr_vel.x = (rand() % 11 - 5); // Random velocity between -2 and 2
			curr_vel.y = (rand() % 11 - 5);
		}

		// 3. Bounce off the walls using the boundaries from GameConfig/BudgetBar
		// If it hits the left or right wall, reverse the X velocity
		if (RefPoint.x < 50 || RefPoint.x > config.windWidth - 50) {
			curr_vel.x = -curr_vel.x;
		}
		// If it hits the top or bottom wall, reverse the Y velocity
		if (RefPoint.y < (config.toolBarHeight * 2) + 50 || RefPoint.y > config.windHeight - config.statusBarHeight - 50) {
			curr_vel.y = -curr_vel.y;
		}
	
	//TO DO: add code for cleanup and game exit here
	cout << "Icon Cow Clicked" << endl;

}

Product* Cow::produceProduct()
{
	point p = { RefPoint.x, RefPoint.y };
	return new Product(pGame, p, 30, 30, "images\\milk.jpg", MILK);
}


Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
	clickCount = 0;
}

void Wolf::moveStep()
{
	// Wolves move faster (velocity between -4 and 4)
	RefPoint.x += curr_vel.x;
	RefPoint.y += curr_vel.y;

	int random_chance = rand() % 100;
	if (random_chance < 2) {
		curr_vel.x = (rand() % 15 - 7); // faster than chick/cow
		curr_vel.y = (rand() % 15 - 7);
	}

	if (RefPoint.x < 50 || RefPoint.x > config.windWidth - 50)
		curr_vel.x = -curr_vel.x;

	if (RefPoint.y < (config.toolBarHeight * 2) + 50 || RefPoint.y > config.windHeight - config.statusBarHeight - 50)
		curr_vel.y = -curr_vel.y;

	cout << "Wolf moving" << endl;
}


void Wolf::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

	// Bar background (gray)
	pWind->SetPen(GRAY, 1);
	pWind->SetBrush(GRAY);
	pWind->DrawRectangle(RefPoint.x, RefPoint.y - 10, RefPoint.x + width, RefPoint.y - 2);

	// Bar fill (red) based on clickCount / 5
	float ratio = (float)clickCount / 5.0f;
	int fillWidth = (int)(width * ratio);
	pWind->SetPen(RED, 1);
	pWind->SetBrush(RED);
	pWind->DrawRectangle(RefPoint.x, RefPoint.y - 10, RefPoint.x + fillWidth, RefPoint.y - 2);
}