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
	warehouse();
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

void Game::showUsernameWindow()
{
	window* uWind = new window(500, 300, 350, 250);

	// Background
	uWind->SetBrush(color(0, 80, 0));
	uWind->SetPen(color(0, 80, 0), 1);
	uWind->DrawRectangle(0, 0, 500, 300);

	// Title bar
	uWind->SetBrush(color(0, 50, 0));
	uWind->SetPen(color(0, 50, 0), 1);
	uWind->DrawRectangle(0, 0, 500, 70);

	// Title
	uWind->SetPen(YELLOW, 1);
	uWind->SetFont(28, BOLD, BY_NAME, "Arial");
	uWind->DrawString(120, 18, "FARM FRENZY");

	// Divider
	uWind->SetPen(YELLOW, 2);
	uWind->DrawLine(20, 75, 480, 75);

	// Welcome text
	uWind->SetPen(WHITE, 1);
	uWind->SetFont(18, PLAIN, BY_NAME, "Arial");
	uWind->DrawString(50, 100, "Welcome! Please enter your name:");

	// Input box background
	uWind->SetBrush(WHITE);
	uWind->SetPen(YELLOW, 2);
	uWind->DrawRectangle(50, 130, 450, 175);

	// Input box text
	uWind->SetPen(BLACK, 1);
	uWind->SetFont(20, BOLD, BY_NAME, "Arial");
	uWind->DrawString(60, 143, "Type and press ENTER");

	// Start button
	uWind->SetBrush(color(255, 165, 0));
	uWind->SetPen(color(200, 120, 0), 2);
	uWind->DrawRectangle(175, 210, 325, 260, FILLED, 10, 10);
	uWind->SetPen(WHITE, 1);
	uWind->SetFont(20, BOLD, BY_NAME, "Arial");
	uWind->DrawString(195, 225, "START GAME");

	// Get username using keyboard
	string name = "";
	char key;
	keytype ktype;
	uWind->FlushKeyQueue();

	while (true)
	{
		// Clear input box
		uWind->SetBrush(WHITE);
		uWind->SetPen(YELLOW, 2);
		uWind->DrawRectangle(50, 130, 450, 175);

		// Show current input
		uWind->SetPen(BLACK, 1);
		uWind->SetFont(20, BOLD, BY_NAME, "Arial");
		uWind->DrawString(60, 143, name + "_");

		ktype = uWind->WaitKeyPress(key);

		if (ktype == ESCAPE)
		{
			name = "Player";
			break;
		}
		if (key == 13)  // ENTER
			break;
		if (key == 8 && name.size() > 0)  // BACKSPACE
			name.resize(name.size() - 1);
		else if (key != 8)
			name += key;
	}

	if (name.empty()) name = "Player";
	username = name;

	delete uWind;
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

void Game::trySpawnWolf(time_t now)
{
	int interval = (level == 1) ? 30 : (level == 2) ? 20 : 10;
	if (now - lastWolfSpawnTime >= interval)
	{
		lastWolfSpawnTime = now;
		spawnWolf();
	}
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

void Game::updateStatusBar()
{
	int count = 0;
	for (int i = 0; i < animalListSize; i++)
		if (animalList[i] != nullptr && dynamic_cast<Wolf*>(animalList[i]) == nullptr)
			count++;
	printMessage("Timer: " + to_string(timer) +
		" | Goal: " + to_string(goal) +
		" | Level: " + to_string(level) +
		" | Animals: " + to_string(count));
}

bool Game::checkLevelUp()
{
	static const int thresholds[] = { 0, 2100, 2300, 2500, 2700, 2900 };
	static const string messages[] = { "", "Level 2 reached!", "Level 3 reached!",
										  "Level 4 reached!", "Level 5 reached!" };

	if (level >= 5 && budget >= thresholds[5])
	{
		printMessage("YOU WIN! Congratulations!");
		pWind->UpdateBuffer();
		Sleep(3000);
		saveScore();
		showLeaderboard();
		return true;  // signal exit
	}

	if (level < 5 && budget >= thresholds[level])
	{
		level++;
		timer = 180 + (level - 1) * 120;
		printMessage(messages[level - 1]);
		Sleep(2000);
	}

	return false;
}

void Game::update()
{
	for (int i = 0; i < animalListSize; i++)
	{
		if (animalList[i] == nullptr) continue;
		animalList[i]->moveStep();

		for (int j = 0; j < foodListSize; j++)
		{
			if (foodList[j] == nullptr || foodList[j]->isEmpty()) continue;
			if (animalList[i]->getRefPoint().x < foodList[j]->getRefPoint().x + foodList[j]->getWidth() &&
				animalList[i]->getRefPoint().x + animalList[i]->getWidth() > foodList[j]->getRefPoint().x &&
				animalList[i]->getRefPoint().y < foodList[j]->getRefPoint().y + foodList[j]->getHeight() &&
				animalList[i]->getRefPoint().y + animalList[i]->getHeight() > foodList[j]->getRefPoint().y)
			{
				foodList[j]->decrease();
				animalList[i]->productCounter += 2;
				if (animalList[i]->productCounter >= animalList[i]->maxCounter)
				{
					animalList[i]->productCounter = 5;
					Product* prod = animalList[i]->produceProduct();
					if (prod != nullptr) addProduct(prod);
				}
			}
		}

		Wolf* wolf = dynamic_cast<Wolf*>(animalList[i]);
		if (wolf == nullptr) continue;
		for (int j = 0; j < animalListSize; j++)
		{
			if (animalList[j] == nullptr || i == j) continue;
			if (dynamic_cast<Wolf*>(animalList[j]) != nullptr) continue;
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

void Game::draw()
{
	pWind->DrawImage("images\\background.jpg", 0, 2 * config.toolBarHeight,
		config.windWidth, config.windHeight - config.statusBarHeight - 2 * config.toolBarHeight);
	warehouse();
	for (int i = 0; i < productListSize; i++)
		if (productList[i] != nullptr && !productList[i]->isCollected)
			productList[i]->draw();
	for (int i = 0; i < foodListSize; i++)
		if (foodList[i] != nullptr && !foodList[i]->isEmpty())
			foodList[i]->draw();
	for (int i = 0; i < animalListSize; i++)
		if (animalList[i] != nullptr)
			animalList[i]->draw();
	pWind->UpdateBuffer();
}

bool Game::tryClickWolf(int x, int y)
{
	for (int i = 0; i < animalListSize; i++)
	{
		if (animalList[i] == nullptr) continue;
		Wolf* wolf = dynamic_cast<Wolf*>(animalList[i]);
		if (wolf == nullptr) continue;
		if (x >= wolf->getRefPoint().x && x <= wolf->getRefPoint().x + wolf->getWidth() &&
			y >= wolf->getRefPoint().y && y <= wolf->getRefPoint().y + wolf->getHeight())
		{
			wolf->clickCount++;
			if (wolf->clickCount >= 5)
			{
				delete animalList[i];
				animalList[i] = nullptr;
			}
			return true;
		}
	}
	return false;
}

bool Game::tryCollectProduct(int x, int y)
{
	for (int i = 0; i < productListSize; i++)
	{
		if (productList[i] == nullptr || productList[i]->isCollected) continue;
		if (x >= productList[i]->getRefPoint().x && x <= productList[i]->getRefPoint().x + productList[i]->getWidth() &&
			y >= productList[i]->getRefPoint().y && y <= productList[i]->getRefPoint().y + productList[i]->getHeight())
		{
			productList[i]->isCollected = true;
			if (productList[i]->type == EGG) warehouseEggs++;
			else if (productList[i]->type == MILK) warehouseMilk++;
			return true;
		}
	}
	return false;
}

void Game::tryPlaceFood(int x, int y)
{
	if (budget >= 20)
	{
		budget -= 20;
		clearBudget();
		string budget_string = "BUDGET = $" + to_string(budget) + " | Animals buying: $100 | Water buying: $20";
		printBudget(budget_string);
		point p; p.x = x; p.y = y;
		foodList[foodListSize++] = new FoodArea(this, p, 80, 80, "images\\grass.jpg");
	}
}

void Game::handleMouseClick(int x, int y)
{
	if (tryClickWolf(x, y)) return;
	if (tryCollectProduct(x, y)) return;
	if (y < config.toolBarHeight) { gameToolbar->handleClick(x, y); return; }
	if (y < 2 * config.toolBarHeight) { gameBudgetbar->handleClick(x, y); return; }
	if (x >= 550 && x <= 660 && y >= 100 && y <= 210) { openWarehouse(); return; }
	tryPlaceFood(x, y);
}

window* Game::getWind() const
{
	return pWind;
}

void Game::go()
{
	pWind->UpdateBuffer();
	showUsernameWindow();


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

				trySpawnWolf(now);
				updateStatusBar();
				if (checkLevelUp()) { isExit = true; }
			}
		}
		if (timer == 0 && !isGameOver)
		{
			isGameOver = true;
			printMessage("TIME'S UP! You lose!");
			pWind->UpdateBuffer();
			Sleep(2000);
			saveScore();
			showLeaderboard();
			isExit = true;
		}

		// ---- Non-blocking mouse check ----
		int x, y;
		clicktype ct = pWind->GetMouseClick(x, y);
		if (ct == LEFT_CLICK || ct == RIGHT_CLICK)
			handleMouseClick(x, y);

		if (!isPaused) update();
		draw();
		Sleep(300);
		pWind->UpdateBuffer();
		Sleep(300);
	 
	} while (!isExit);
}