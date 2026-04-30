#include "Product.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

Product::Product(Game* r_pGame, point r_point, int r_width, int r_height, string img_path, ProductType t)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
    type = t;
}

void Product::draw() const
{
    if (isCollected) return;
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}