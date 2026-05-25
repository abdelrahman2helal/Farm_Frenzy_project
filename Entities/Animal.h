#pragma once
#include "../Core/Drawable.h"
#include "Product.h"


class Animal :public Drawable
{
protected:
	string image_path;
public:
	Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	point curr_pos;
	point curr_vel;
	Product* myProduct = nullptr; // the product this animal produced
	int productCounter = 5;      // counts up every second
	int maxCounter;              // max before producing (10 for chick, 15 for cow)
	virtual Product* produceProduct() = 0; // creates egg or milk
	virtual void draw() const override;
	virtual void moveStep() = 0;   //The action that should be taken each time step
	void incrementCounter();     // call every second
	void decreaseHunger();
};

class Chick : public Animal
{
public:
	Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep();
	virtual Product* produceProduct() ;
};

class Cow : public Animal
{
public:
	Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep();
	virtual Product* produceProduct() ;
};

class Wolf : public Animal
{
public:
	int clickCount = 0;
	Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep();
	virtual Product* produceProduct() { return nullptr; }
	virtual void draw() const override;
};