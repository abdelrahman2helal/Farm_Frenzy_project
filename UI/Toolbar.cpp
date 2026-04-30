#include "Toolbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include "../Entities/Animal.h"
#include "../Entities/FoodArea.h"
#include <iostream>
#include <fstream>
using namespace std;

ToolbarIcon::ToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void ToolbarIcon::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}
void PauseIcon::onClick()
{
	pGame->isPaused = true;
	//TO DO: add code for pausing the game here
}

ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}
void ResumeIcon::onClick()
{
	pGame->isPaused = false;
	//TO DO: add code for resuming the game here
}

SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}


void SaveIcon::onClick()
{
	ofstream outFile("savegame.txt");
	if (!outFile) return;

	outFile << pGame->budget << "\n";
	outFile << pGame->timer << "\n";
	outFile << pGame->level << "\n";
	outFile << pGame->animalCount << "\n";

	// Save animals
	outFile << pGame->animalListSize << "\n";
	for (int i = 0; i < pGame->animalListSize; i++)
	{
		if (pGame->animalList[i] != nullptr)
		{
			// Save type: 0=Chick, 1=Cow, 2=Wolf
			Chick* chick = dynamic_cast<Chick*>(pGame->animalList[i]);
			Cow* cow = dynamic_cast<Cow*>(pGame->animalList[i]);
			Wolf* wolf = dynamic_cast<Wolf*>(pGame->animalList[i]);

			int type = chick ? 0 : (cow ? 1 : 2);
			outFile << type << " "
				<< pGame->animalList[i]->getRefPoint().x << " "
				<< pGame->animalList[i]->getRefPoint().y << "\n";
		}
	}

	// Save food areas
	outFile << pGame->foodListSize << "\n";
	for (int i = 0; i < pGame->foodListSize; i++)
	{
		if (pGame->foodList[i] != nullptr)
		{
			outFile << pGame->foodList[i]->getRefPoint().x << " "
				<< pGame->foodList[i]->getRefPoint().y << " "
				<< pGame->foodList[i]->foodCount << "\n";
		}
	}

	outFile.close();
	cout << "Game saved!" << endl;
}


GLoadIcon::GLoadIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}
void GLoadIcon::onClick()
{
	ifstream inFile("savegame.txt");
	if (!inFile) return;

	inFile >> pGame->budget;
	inFile >> pGame->timer;
	inFile >> pGame->level;
	inFile >> pGame->animalCount;

	// Clear existing animals
	for (int i = 0; i < pGame->animalListSize; i++)
	{
		delete pGame->animalList[i];
		pGame->animalList[i] = nullptr;
	}
	pGame->animalListSize = 0;

	// Load animals
	int animalCount;
	inFile >> animalCount;
	for (int i = 0; i < animalCount; i++)
	{
		int type, x, y;
		inFile >> type >> x >> y;
		point p; p.x = x; p.y = y;

		Animal* animal = nullptr;
		if (type == 0) animal = new Chick(pGame, p, 50, 50, "images\\chick.jpg");
		else if (type == 1) animal = new Cow(pGame, p, 60, 60, "images\\cow.jpg");
		else animal = new Wolf(pGame, p, 70, 70, "images\\wolf.jpg");

		pGame->animalList[pGame->animalListSize++] = animal;
	}

	// Clear existing food
	for (int i = 0; i < pGame->foodListSize; i++)
	{
		delete pGame->foodList[i];
		pGame->foodList[i] = nullptr;
	}
	pGame->foodListSize = 0;

	// Load food areas
	int foodCount;
	inFile >> foodCount;
	for (int i = 0; i < foodCount; i++)
	{
		int x, y, count;
		inFile >> x >> y >> count;
		point p; p.x = x; p.y = y;
		FoodArea* food = new FoodArea(pGame, p, 80, 80, "images\\grass.jpg");
		food->foodCount = count;
		pGame->foodList[pGame->foodListSize++] = food;
	}

	inFile.close();
	cout << "Game loaded!" << endl;

	string budget_string = "BUDGET = $" + to_string(pGame->budget) + " | Animals buying: $100 | Water buying: $20";
	pGame->printBudget(budget_string);
}

RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{}

void RestartIcon::onClick()
{
	// Reset game state
	pGame->budget = 2000;
	pGame->timer = 120;
	pGame->animalCount = 0;
	pGame->isPaused = false;

	// Clear all animals
	for (int i = 0; i < pGame->animalListSize; i++)
	{
		delete pGame->animalList[i];
		pGame->animalList[i] = nullptr;
	}
	pGame->animalListSize = 0;

	// Clear all food
	for (int i = 0; i < pGame->foodListSize; i++)
	{
		delete pGame->foodList[i];
		pGame->foodList[i] = nullptr;
	}
	pGame->foodListSize = 0;

	// Clear all products
	for (int i = 0; i < pGame->productListSize; i++)
	{
		delete pGame->productList[i];
		pGame->productList[i] = nullptr;
	}
	pGame->productListSize = 0;

	// Redraw background
	window* pWind = pGame->getWind();
	pWind->DrawImage("images\\background.jpg", 0, 0, pWind->GetWidth(), pWind->GetHeight());
	pGame->createToolbar();
	pGame->createBudgetbar();
	pGame->warehouse();
	pGame->clearStatusBar();

	string budget_string = "BUDGET = $" + to_string(pGame->budget) + " | Animals buying: $100 | Water buying: $20";
	pGame->printBudget(budget_string);
}

ExitIcon::ExitIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{}

void ExitIcon::onClick()
{
	//TO DO: add code for cleanup and game exit here
}

Toolbar::Toolbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	//First prepare List of images for each icon
	//To control the order of these images in the menu, reoder them in enum ICONS above	
	iconsImages[ICON_PAUSE] = "images\\pause.jpg";
	iconsImages[ICON_RESUME] = "images\\RESUME.jpg";
	iconsImages[ICON_SAVE] = "images\\SAVE.jpg";
	iconsImages[ICON_LOAD] = "images\\LOAD.jpg";
	iconsImages[ICON_RESTART] = "images\\RESTART.jpg";
	iconsImages[ICON_EXIT] = "images\\EXIT.jpg";
	point p;
	p.x = 0;
	p.y = 0;

	iconsList = new ToolbarIcon * [ICON_COUNT];

	//For each icon in the tool bar create an object 
	iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESTART]);
	p.x += config.iconWidth;
	iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);
	p.x += config.iconWidth;
	iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);
	p.x += config.iconWidth;
	iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);
	p.x += config.iconWidth;
	iconsList[ICON_LOAD] = new GLoadIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);
	p.x += config.iconWidth;
	iconsList[ICON_EXIT] = new ExitIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_EXIT]);
}


Toolbar::~Toolbar()
{
	for (int i = 0; i < ICON_COUNT; i++)
		delete iconsList[i];
	delete iconsList;
}

void Toolbar::draw() const
{
	for (int i = 0; i < ICON_COUNT; i++)
		iconsList[i]->draw();
	window* pWind = pGame->getWind();
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
}

bool Toolbar::handleClick(int x, int y)
{
	if (x > ICON_COUNT * config.iconWidth)	//click outside toolbar boundaries
		return false;


	//Check whick icon was clicked
	//==> This assumes that menu icons are lined up horizontally <==
	//Divide x co-ord of the point clicked by the icon width (int division)
	//if division result is 0 ==> first icon is clicked, if 1 ==> 2nd icon and so on

	int clickedIconIndex = (x / config.iconWidth);
	iconsList[clickedIconIndex]->onClick();	//execute onClick action of clicled icon

	if (clickedIconIndex == ICON_EXIT) return true;

	return false;

}

