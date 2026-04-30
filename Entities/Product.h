#pragma once
#include "../Core/Drawable.h"

enum ProductType { EGG, MILK };

class Product : public Drawable
{
private:
    string image_path;
public:
    bool isCollected = false;
    ProductType type;
    Product(Game* r_pGame, point r_point, int r_width, int r_height, string img_path, ProductType t);
    virtual void draw() const override;
};