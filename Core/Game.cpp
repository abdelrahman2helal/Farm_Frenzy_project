#include "Game.h"
#include "../Config/GameConfig.h"
#include <ctime> 
#include <iostream>
#include <windows.h> 
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

Game::Game()
{
	budget = 2000;
	level = 1;

	if (level == 1) { goal = 5; }
	else if (level == 2) { goal = 10; }
	else if (level == 3) { goal = 15; }
	else { goal = 5; }

	timer = 180 + (level - 1) * 120;


	animalCount = 0;
	//1 - Create the main window
	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);

	//2 - create and draw the toolbar
	createToolbar();
	createBudgetbar();
	//3 - create and draw the backgroundPlayingArea


	//4- Create the Plane
	warehouse();

	//5- Create the Bullet
	//TODO: Add code to create and draw the Bullet

	//6- Create the enemies
	//TODO: Add code to create and draw enemies in random places

	//7- Create and clear the status bar
	clearStatusBar();

	animalList = new Animal * [50];
	for (int i = 0; i < 50; i++)
		animalList[i] = nullptr;

	foodList = new FoodArea * [50];
	for (int i = 0; i < 50; i++)
		foodList[i] = nullptr;

	productList = new Product * [100];
	for (int i = 0; i < 100; i++)
		productList[i] = nullptr;

	pWind->UpdateBuffer();
}

Game::~Game()
{
}

clicktype Game::getMouseClick(int& x, int& y) const
{
	return pWind->GetMouseClick(x, y);	//Wait for mouse click to get mouse click so it doesnt wait
}

string Game::getSrting() const
{
	string Label;
	char Key;
	keytype ktype;
	pWind->FlushKeyQueue();
	while (1)
	{
		ktype = pWind->WaitKeyPress(Key);
		if (ktype == ESCAPE)	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if (Key == 13)	//ENTER key is pressed
			return Label;
		if (Key == 8)	//BackSpace is pressed
			if (Label.size() > 0)
				Label.resize(Label.size() - 1);
			else
				Key = '\0';
		else
			Label += Key;
		printMessage(Label);
	}
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
	window* pW = new window(w, h, x, y);
	pW->SetBrush(config.bkGrndColor);
	pW->SetPen(config.bkGrndColor, 1);
	pW->DrawRectangle(0, 0, w, h);
	pW->DrawImage("images\\background.jpg", 0, 0, w, h); // Draw background image
	pW->SetBuffering(true);
	return pW;
}

void Game::addProduct(Product* p)
{
	if (p == nullptr) return;
	productList[productListSize++] = p;
}

void Game::warehouse() const    // Draw the warehouse in the upper right corner of the playing area
{
	window* pWind = getWind();  
	pWind->DrawImage("images\\warehouse.jpg", 550, 100, 110, 110);

}

void Game::saveScore()
{
	// Read existing scores
	ifstream inFile("leaderboard.txt");
	vector<pair<string, int>> scores;
	string name;
	int score;
	while (inFile >> name >> score)
		scores.push_back({ name, score });
	inFile.close();

	// Add current score
	scores.push_back({ username, budget });

	// Sort by score descending
	sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
		return a.second > b.second;
		});

	// Keep top 10
	if (scores.size() > 10)
		scores.resize(10);

	// Write back
	ofstream outFile("leaderboard.txt");
	for (auto& s : scores)
		outFile << s.first << " " << s.second << "\n";
	outFile.close();
}

void Game::showLeaderboard()
{
	window* lWind = new window(400, 500, 400, 150);
	lWind->SetBrush(color(0, 80, 0));
	lWind->SetPen(color(0, 80, 0), 1);
	lWind->DrawRectangle(0, 0, 400, 500);

	lWind->SetPen(YELLOW, 1);
	lWind->SetFont(24, BOLD, BY_NAME, "Arial");
	lWind->DrawString(120, 10, "LEADERBOARD");

	lWind->SetPen(YELLOW, 2);
	lWind->DrawLine(20, 45, 380, 45);

	ifstream inFile("leaderboard.txt");
	string name;
	int score;
	int rank = 1;
	int y = 60;
	while (inFile >> name >> score && rank <= 10)
	{
		string line = to_string(rank) + ". " + name + " - $" + to_string(score);
		if (name == username)
			lWind->SetPen(YELLOW, 1);  // highlight current player
		else
			lWind->SetPen(WHITE, 1);
		lWind->SetFont(18, BOLD, BY_NAME, "Arial");
		lWind->DrawString(20, y, line);
		y += 35;
		rank++;
	}
	inFile.close();

	lWind->SetPen(WHITE, 1);
	lWind->SetFont(16, PLAIN, BY_NAME, "Arial");
	lWind->DrawString(120, 460, "Click to close");

	int wx, wy;
	lWind->WaitMouseClick(wx, wy);
	delete lWind;
}

void Game::openWarehouse()
{
	// Create window
	window* wWind = new window(500, 400, 350, 200);

	// Dark green background
	wWind->SetBrush(GREEN);
	wWind->SetPen(GREEN, 1);
	wWind->DrawRectangle(0, 0, 500, 400);

	// Title bar - darker green
	wWind->SetBrush(color(0, 80, 0));
	wWind->SetPen(color(0, 80, 0), 1);
	wWind->DrawRectangle(0, 0, 500, 60);

	// Title text
	wWind->SetPen(YELLOW, 1);
	wWind->SetFont(28, BOLD, BY_NAME, "Arial");
	wWind->DrawString(160, 15, "WAREHOUSE");

	// Divider line
	wWind->SetPen(YELLOW, 2);
	wWind->DrawLine(20, 65, 480, 65);

	// Egg section background
	wWind->SetBrush(color(0, 100, 0));
	wWind->SetPen(color(0, 100, 0), 1);
	wWind->DrawRectangle(20, 80, 480, 175);

	// Egg image and info
	wWind->DrawImage("images\\egg.jpg", 30, 90, 70, 70);
	wWind->SetPen(WHITE, 1);
	wWind->SetFont(20, BOLD, BY_NAME, "Arial");
	wWind->DrawString(120, 95, "Eggs");
	wWind->SetFont(18, PLAIN, BY_NAME, "Arial");
	wWind->DrawString(120, 125, "Count: " + to_string(warehouseEggs));
	wWind->DrawString(120, 150, "Price: $25 each");

	// Sell eggs button
	wWind->SetBrush(color(255, 165, 0));
	wWind->SetPen(color(200, 120, 0), 2);
	wWind->DrawRectangle(360, 100, 470, 155, FILLED, 10, 10);
	wWind->SetPen(WHITE, 1);
	wWind->SetFont(18, BOLD, BY_NAME, "Arial");
	wWind->DrawString(385, 118, "SELL");
	wWind->SetFont(14, PLAIN, BY_NAME, "Arial");
	wWind->DrawString(372, 140, "$" + to_string(warehouseEggs * 25));

	// Milk section background
	wWind->SetBrush(color(0, 100, 0));
	wWind->SetPen(color(0, 100, 0), 1);
	wWind->DrawRectangle(20, 190, 480, 285);

	// Milk image and info
	wWind->DrawImage("images\\milk.jpg", 30, 200, 70, 70);
	wWind->SetPen(WHITE, 1);
	wWind->SetFont(20, BOLD, BY_NAME, "Arial");
	wWind->DrawString(120, 205, "Milk");
	wWind->SetFont(18, PLAIN, BY_NAME, "Arial");
	wWind->DrawString(120, 235, "Count: " + to_string(warehouseMilk));
	wWind->DrawString(120, 260, "Price: $50 each");

	// Sell milk button
	wWind->SetBrush(color(255, 165, 0));
	wWind->SetPen(color(200, 120, 0), 2);
	wWind->DrawRectangle(360, 210, 470, 265, FILLED, 10, 10);
	wWind->SetPen(WHITE, 1);
	wWind->SetFont(18, BOLD, BY_NAME, "Arial");
	wWind->DrawString(385, 228, "SELL");
	wWind->SetFont(14, PLAIN, BY_NAME, "Arial");
	wWind->DrawString(372, 250, "$" + to_string(warehouseMilk * 50));

	// Close button
	wWind->SetBrush(RED);
	wWind->SetPen(RED, 2);
	wWind->DrawRectangle(190, 320, 310, 370, FILLED, 10, 10);
	wWind->SetPen(WHITE, 1);
	wWind->SetFont(18, BOLD, BY_NAME, "Arial");
	wWind->DrawString(220, 335, "CLOSE");

	// Divider line
	wWind->SetPen(YELLOW, 2);
	wWind->DrawLine(20, 310, 480, 310);

	// Handle clicks
	int wx, wy;
	wWind->WaitMouseClick(wx, wy);

	// Sell eggs
	if (wx >= 360 && wx <= 470 && wy >= 100 && wy <= 155)
	{
		budget += warehouseEggs * 25;
		warehouseEggs = 0;
		clearBudget();
		string budget_string = "BUDGET = $" + to_string(budget) + " | Animals buying: $100 | Water buying: $20";
		printBudget(budget_string);
		pWind->UpdateBuffer();
	}

	// Sell milk
	if (wx >= 360 && wx <= 470 && wy >= 210 && wy <= 265)
	{
		budget += warehouseMilk * 50;
		warehouseMilk = 0;
		clearBudget();
		string budget_string = "BUDGET = $" + to_string(budget) + " | Animals buying: $100 | Water buying: $20";
		printBudget(budget_string);
		pWind->UpdateBuffer();
	}

	delete wWind;
}

void Game::spawnWolf()
{
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<int> distx(50, config.windWidth - 50);

	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<int> disty((config.toolBarHeight * 2) + 50, config.windHeight - config.statusBarHeight - 50);

	point p;
	p.x = distx(gen1);
	p.y = disty(gen2);

	animalList[animalListSize] = new Wolf(this, p, 70, 70, "images\\wolf.jpg");
	animalListSize++;
}

void Game::createToolbar() 
{
	point toolbarUpperleft;
	toolbarUpperleft.x = 0;
	toolbarUpperleft.y = 0;

	gameToolbar = new Toolbar(this, toolbarUpperleft, 0, config.toolBarHeight);
	gameToolbar->draw();
}

void Game::createBudgetbar()
{
	point budgetbarUpperleft;
	budgetbarUpperleft.x = 0;
	budgetbarUpperleft.y = config.toolBarHeight;

	gameBudgetbar = new Budgetbar(this, budgetbarUpperleft, 0, config.toolBarHeight);
	gameBudgetbar->draw();
}


void Game::clearBudget() const
{
	//Clear Status bar by drawing a filled rectangle
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight, config.windWidth, 2*config.toolBarHeight);
}

void Game::printBudget(string msg) const
{
	clearBudget();	//First clear the status bar

	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth-600, config.toolBarHeight + 10, msg);

}

void Game::clearStatusBar() const
{
	//Clear Status bar by drawing a filled rectangle
	pWind->SetPen(config.statusBarColor, 1);
	pWind->SetBrush(config.statusBarColor);
	pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight, config.windWidth, config.windHeight);
}

void Game::printMessage(string msg) const
{
	clearStatusBar();	//First clear the status bar

	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);

}


window* Game::getWind() const
{
	return pWind;
}


void Game::go()
{
	printMessage("Enter your name and press ENTER:");
	pWind->UpdateBuffer();
	username = getSrting();


	int x, y;
	bool isExit = false;

	pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");
	lastWolfSpawnTime = time(0);
	lastTickTime = time(0);

	// Draw budget once at the start
	string budget_string = "BUDGET = $" + to_string(budget) + " | Animals buying: $100 | Water buying: $20";
	printBudget(budget_string);

	do
	{
		
		// ---- Timer logic ----
		time_t now = time(0);
		if (!isPaused)  // only update if not paused
		{
			if (now - lastTickTime >= 1)
			{
				lastTickTime = now;
				if (timer > 0)
					timer--;

				for (int i = 0; i < animalListSize; i++)
					if (animalList[i] != nullptr)
					{

						Wolf* wolf = dynamic_cast<Wolf*>(animalList[i]);
						if (wolf == nullptr)  // only decrease hunger for non-wolves
							animalList[i]->incrementCounter();
					}


				int wolfSpawnInterval;
				if (level == 1) wolfSpawnInterval = 30;  // every 30 seconds
				else if (level == 2) wolfSpawnInterval = 20; // every 20 seconds
				else wolfSpawnInterval = 10; // every 10 seconds

				if (now - lastWolfSpawnTime >= wolfSpawnInterval)
				{
					lastWolfSpawnTime = now;
					spawnWolf();
				}

				// Only update status bar once per second
				string status = "Timer: " + to_string(timer) +
					" | Goal: " + to_string(goal) +
					" | Level: " + to_string(level) +
					" | Animals: " + to_string(animalCount);
				printMessage(status);
				// Check if budget goal reached to increase level
				if (level == 1 && budget >= 2100)
				{
					level++;
					printMessage("Level 2 reached! Goal: $4000");
					Sleep(2000);
				}
				else if (level == 2 && budget >= 2300)
				{
					level++;
					printMessage("Level 3 reached! Goal: $5000");
					Sleep(2000);
				}
				else if (level == 3 && budget >= 2500)
				{
					level++;
					printMessage("Level 4 reached! Goal: $8000");
					Sleep(2000);
				}
				else if (level == 4 && budget >= 2700)
				{
					level++;
					printMessage("Level 5 reached! Goal: $10000");
					Sleep(2000);
				}
				else if (level == 5 && budget >= 2900)
				{
					printMessage("YOU WIN! Congratulations!");
					Sleep(3000);
					isExit = true;
				}
			}


			if (timer == 0)
			{
				printMessage("TIME'S UP! You lose!");
				Sleep(2000);
				saveScore();
				showLeaderboard();
				break;
			}
		}
		// ---- Non-blocking mouse check ----
		// ---- Non-blocking mouse check ----
		clicktype ct = pWind->GetMouseClick(x, y);
		if (ct == LEFT_CLICK || ct == RIGHT_CLICK)
		{
			// Check if a wolf was clicked
			bool wolfClicked = false;
			for (int i = 0; i < animalListSize; i++)
			{
				if (animalList[i] != nullptr)
				{
					Wolf* wolf = dynamic_cast<Wolf*>(animalList[i]);
					if (wolf != nullptr)
					{
						if (x >= wolf->getRefPoint().x &&
							x <= wolf->getRefPoint().x + wolf->getWidth() &&
							y >= wolf->getRefPoint().y &&
							y <= wolf->getRefPoint().y + wolf->getHeight())
						{
							wolf->clickCount++;
							cout << "Wolf clicked! Count: " << wolf->clickCount << endl;
							wolfClicked = true;
							if (wolf->clickCount >= 5)
							{
								delete animalList[i];
								animalList[i] = nullptr;
							}
							break;
						}
					}
				}
			}

			if (!wolfClicked)
			{
				// check if a product was clicked
				bool productClicked = false;
				for (int i = 0; i < productListSize; i++)
				{
					if (productList[i] != nullptr && !productList[i]->isCollected)
					{
						if (x >= productList[i]->getRefPoint().x &&
							x <= productList[i]->getRefPoint().x + productList[i]->getWidth() &&
							y >= productList[i]->getRefPoint().y &&
							y <= productList[i]->getRefPoint().y + productList[i]->getHeight())
						{
							productList[i]->isCollected = true;
							if (productList[i]->type == EGG)
								warehouseEggs++;
							else if (productList[i]->type == MILK)
								warehouseMilk++;
							productClicked = true;
							break;
						}
					}
				}
				if (!productClicked)
				{
					if (y >= 0 && y < config.toolBarHeight)
						isExit = gameToolbar->handleClick(x, y);
					else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
						isExit = gameBudgetbar->handleClick(x, y);
					else if (x >= 550 && x <= 660 && y >= 100 && y <= 210)
						openWarehouse();
					else  // clicked on field
					{
						if (budget >= 20)
						{
							budget -= 20;
							clearBudget();
							string budget_string = "BUDGET = $" + to_string(budget) + " | Animals buying: $100 | Water buying: $20";
							printBudget(budget_string);

							point p; p.x = x; p.y = y;
							foodList[foodListSize] = new FoodArea(this, p, 80, 80, "images\\grass.jpg");
							foodListSize++;
						}
					}
				}
			}
		}
			

		pWind->DrawImage("images\\background.jpg", 0, 2 * config.toolBarHeight,
				config.windWidth, config.windHeight - config.statusBarHeight - 2 * config.toolBarHeight);

		warehouse();
		
		// Draw products (eggs/milk)
		for (int i = 0; i < productListSize; i++)
			if (productList[i] != nullptr && !productList[i]->isCollected)
				productList[i]->draw();
	
		for (int i = 0; i < foodListSize; i++)
			if (foodList[i] != nullptr && !foodList[i]->isEmpty())
				foodList[i]->draw();



		for (int i = 0; i < animalListSize; i++)
		{
			if (animalList[i] != nullptr)
			{
				if (animalList[i]->isDead)
				{
					delete animalList[i];
					animalList[i] = nullptr;
					continue;
				}
				if (!isPaused)
				{
					animalList[i]->moveStep();

					for (int j = 0; j < foodListSize; j++)
					{
						if (foodList[j] != nullptr && !foodList[j]->isEmpty())
						{
							if (animalList[i]->getRefPoint().x < foodList[j]->getRefPoint().x + foodList[j]->getWidth() &&
								animalList[i]->getRefPoint().x + animalList[i]->getWidth() > foodList[j]->getRefPoint().x &&
								animalList[i]->getRefPoint().y < foodList[j]->getRefPoint().y + foodList[j]->getHeight() &&
								animalList[i]->getRefPoint().y + animalList[i]->getHeight() > foodList[j]->getRefPoint().y)
							{
								foodList[j]->decrease();
								animalList[i]->productCounter += 2;  // eating increases counter
								if (animalList[i]->productCounter >= animalList[i]->maxCounter)
								{
									animalList[i]->productCounter = 5;  // reset after producing
									Animal* a = animalList[i];
									Product* prod = a->produceProduct();
									if (prod != nullptr)
										addProduct(prod);
								}

							}
						}
					}

					Wolf* wolf = dynamic_cast<Wolf*>(animalList[i]);
					if (wolf != nullptr)
					{
						for (int j = 0; j < animalListSize; j++)
						{
							if (animalList[j] != nullptr && i != j)
							{
								Wolf* otherWolf = dynamic_cast<Wolf*>(animalList[j]);
								if (otherWolf == nullptr)
								{
									if (wolf->getRefPoint().x < animalList[j]->getRefPoint().x + animalList[j]->getWidth() &&
										wolf->getRefPoint().x + wolf->getWidth() > animalList[j]->getRefPoint().x &&
										wolf->getRefPoint().y < animalList[j]->getRefPoint().y + animalList[j]->getHeight() &&
										wolf->getRefPoint().y + wolf->getHeight() > animalList[j]->getRefPoint().y)
									{
										delete animalList[j];
										animalList[j] = nullptr;
									}
								}
							}
						}
					}
				}
				animalList[i]->draw(); // always draw
			}
		}
		pWind->UpdateBuffer();
		Sleep(300);

	} while (!isExit);
}