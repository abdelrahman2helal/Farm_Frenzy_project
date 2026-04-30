#pragma once
#include "../Core/Drawable.h"

class FoodArea : public Drawable
{
private:
    string image_path;
public:
    int foodCount;
    bool isEmpty() const { return foodCount <= 0; }
    void decrease() { if (foodCount > 0) foodCount--; }
    FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
};