#include "FoodArea.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

FoodArea::FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
    foodCount = 10;
}


void FoodArea::draw() const
{
    if (foodCount <= 0) return; // don't draw if empty

    window* pWind = pGame->getWind();

    // Draw grass image
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

    // Draw counter number on top of grass
    pWind->SetPen(BLACK, 1);
    pWind->SetFont(20, BOLD, BY_NAME, "Arial");
    pWind->DrawString(RefPoint.x + width / 2 - 5, RefPoint.y + 5, to_string(foodCount));
}