#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../Entities/Animal.h"
#include "../UI/BudgetBar.h"
#include "../Entities/FoodArea.h"
#include "../Entities/Product.h"
#include <ctime>

class Game
{
private:
	window* pWind;	//Pointer to the CMU graphics window
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;
	
public:
	time_t lastTickTime;
	Animal** animalList;
	FoodArea** foodList;
	Product** productList;
	time_t lastWolfSpawnTime;
	string username;
	int productListSize = 0;
	int foodListSize = 0;
	int animalListSize = 0;
	int budget = 2000;
	int timer;
	int goal;
	int level;
	int animalCount;
	int warehouseEggs = 0;
	int warehouseMilk = 0;
	bool isPaused = false;
	bool isGameOver = false;

	Game();
	~Game();
	clicktype getMouseClick(int& x, int& y) const; //Get coordinate where user clicks and returns click type (left/right)
	string getSrting() const;	 //Returns a string entered by the user
	window* CreateWind(int, int, int, int) const; //creates the game window
	void createToolbar();
	void createBudgetbar();
	void clearBudget() const;
	void warehouse() const;
	void saveScore();
	void showLeaderboard();
	void showUsernameWindow();
	void spawnWolf();
	void openWarehouse();
	void addProduct(Product* p);
	void printBudget(string msg) const;
	void clearStatusBar() const;	//Clears the status bar
	void printMessage(string msg) const;	//Print a message on Status bar
	void trySpawnWolf(time_t now);
	void updateStatusBar();
	bool checkLevelUp();
	void handleMouseClick(int x, int y);
	bool tryClickWolf(int x, int y);
	bool tryCollectProduct(int x, int y);
	void tryPlaceFood(int x, int y);
	void update();   // all movement and collision logic
	void draw();     // all rendering
	void go() ;
	window* getWind() const;		//returns a pointer to the graphics window
};