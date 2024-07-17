#include <iostream>
#include <termios.h>

using std::cout, std::cin, std::string, std::endl, std::stoi;

#define RED_BOLD "\033[31;1m"
#define BLUE_BOLD "\033[34;1m"
#define INVERTED_COLORS "\033[30;47m"
#define RESET_COLORS "\033[0m"
#define ENTER_ALTERNATE_BUFFER "\033[?1049h"
#define EXIT_ALTERNATE_BUFFER "\033[?1049l"
#define RESET_CURSOR_POSITION "\033[H"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"

const int ROWS = 3;
const int COLUMNS = 3;
const int TOTAL_CELLS = ROWS * COLUMNS;
char board[ROWS][COLUMNS];
const string COMPUTER_NAME = "Computer";
int matches;

struct Player
{
	string name;
	char move;
	int win;
};

Player p1{.move = 'X'};
Player p2{.move = 'O'};

void setRawMode(bool enable);
bool startGame(char gameMode);
void playerNames();
void printMenu(int menuChoice);
void printOption(int option);
void printBoard();
void playerMove(Player player);
void placeMarker(Player & player, int position);
bool computerMove();
bool checkWin(Player & player);
bool checkTie(int moveCount);
void resetBoard();
void showWins();
bool playAgain(string result);
void clearInputBuffer();

int main()
{
	int menuChoice;
	char gameMode;

	cout << ENTER_ALTERNATE_BUFFER;
	cout << RESET_CURSOR_POSITION;
		
	while (true)
	{   
		menuChoice = 0;
		p1.win = 0;
		p2.win = 0;
		matches = 0;

		cout << HIDE_CURSOR;
		setRawMode(true);

		while (true)
		{
			printMenu(menuChoice);

			int ch = getchar();

			if (ch == '\e')
			{
				ch = getchar();
				if (ch == '[')
				{
					ch = getchar();

					if (ch == 'A' && menuChoice > 0)
						menuChoice--;
					else if (ch == 'B' && menuChoice < 2) 
						menuChoice++;
				}
			} 
			else if (ch == '\n') 
			{
				break;
			} 
			else 
			{

			}
		}

		setRawMode(false);
		cout << SHOW_CURSOR;

		if (menuChoice == 2)
			{
				break;
			}

		switch (menuChoice)
		{
			case 0:
				gameMode = 'p';
				p2.name = "";
				resetBoard();
				playerNames();
				break;
			case 1:
				gameMode = 'c';
				p2.name = COMPUTER_NAME;
				resetBoard();
				playerNames();
				break;
		}

		while (startGame(gameMode))
		{
			resetBoard();
		}
	}

	cout << EXIT_ALTERNATE_BUFFER;
		
	return 0;
}

void setRawMode(bool enable)
{
	static struct termios oldt, newt;
	if (enable)
	{
		tcgetattr(0, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(0, TCSANOW, &newt);
	}
	else
	{
		tcsetattr(0, TCSANOW, &oldt);
	}
}

bool startGame(char gameMode)
{
	int moveCount = 0;
	string result;

	do
	{
		printBoard();
		showWins();
		moveCount++;

		if (matches % 2 == 0)
		{
			playerMove(p1);
			if (checkWin(p1))
			{
				result = p1.name + " won!!\n";
				break;
			}
		}
		else if (gameMode == 'p' && matches % 2 == 1)
		{
			playerMove(p2);
			if (checkWin(p2))
			{
				result = p2.name + " won!!\n";
				break;
			}
		}
		else
		{
			computerMove();
			if (checkWin(p2))
			{
				result = p2.name + " won!!\n";
				break;
			}
		}

		if (checkTie(moveCount))
		{
			result = "It's a tie.\n";
			break;
		}

		printBoard();
		showWins();
		moveCount++;

		if (matches % 2 == 1)
		{
			playerMove(p1);
			if (checkWin(p1))
			{
				result = p1.name + " won!!\n";
				break;
			}
		}
		else if (gameMode == 'p' && matches % 2 == 0)
		{
			playerMove(p2);
			if (checkWin(p2))
			{
				result = p2.name + " won!!\n";
				break;
			}
		}
		else
		{
			computerMove();
			if (checkWin(p2))
			{
				result = p2.name + " won!!\n";
				break;
			}
		}
	}
	while (true);

	matches++;

	return playAgain(result);
}

void playerNames()
{
	system("clear");
		
	if (p2.name == COMPUTER_NAME)
	{
		cout << "------Tic-Tac-Toe------\n";
		cout << "Insert your name:\n";
		cout << "--> ";
		cin >> p1.name;
	}

	else
	{
		cout << "------Tic-Tac-Toe------\n";
		cout << "Insert FIRST player's name:\n";
		cout << "--> ";
		cin >> p1.name;

		cout << "\nInsert SECOND player's name:\n";
		cout << "--> ";
		cin >> p2.name;
	}

	clearInputBuffer();
}

void printBoard()
{
	system("clear");

	cout << "------Tic-Tac-Toe------\n\n";

	for (int i = 0; i < ROWS; i++)
	{
		cout << "      ";
		for (int j = 0; j < COLUMNS; j++)
		{
			if (board[i][j] == 'X')
				cout << RED_BOLD << " " << board[i][j] << " " << RESET_COLORS;
			else if (board[i][j] == 'O')
				cout << BLUE_BOLD << " " << board[i][j] << " " << RESET_COLORS;
			else
				cout << " " << board[i][j] << " ";

			if (j < COLUMNS - 1)
				cout << "|";
		}

		cout << "\n";

		if (i < ROWS - 1)
		{
			cout << "      " << "-----------\n";
		}
	}

	cout << "\n";
}

void printMenu(int menuChoice)
{
	cout << endl;
	system("clear");

	struct colors
	{
		string invert;
		string reset;
	};

	colors option1{.invert = "", .reset = ""};
	colors option2{.invert = "", .reset = ""};
	colors option3{.invert = "", .reset = ""};

	switch (menuChoice)
	{
		case 0:
			option1.invert = INVERTED_COLORS;
			option1.reset = RESET_COLORS;
			break;
		case 1:
			option2.invert = INVERTED_COLORS;
			option2.reset = RESET_COLORS;
			break;
		case 2:
			option3.invert = INVERTED_COLORS;
			option3.reset = RESET_COLORS;
			break;
	}

	cout << "------Tic-Tac-Toe------\n";
	cout << "  " << option1.invert << "Player vs Player" << option1.reset << endl;
	cout << "  " << option2.invert << "Player vs Computer" << option2.reset << endl;
	cout << "  " << option3.invert << "Exit" << option3.reset << endl;
	cout << "-----------------------";
}

void printOption(int option)
{
	if (option == 0)
		cout << "Do you want to play again? [" << INVERTED_COLORS << "y" << RESET_COLORS << "/n]";
	else
		cout << "Do you want to play again? [y/" << INVERTED_COLORS << "n" << RESET_COLORS << "]";
}

void playerMove(Player player)
{
	int position;

	cout << player.name << "'s turn\n";
	cout << "Choose where to place your \"" << player.move << "\": ";
	cout.flush();
	
	setRawMode(true);
	position = getchar() - '0';
	setRawMode(false);

	placeMarker(player, position);
}

void placeMarker(Player & player, int position)
{
	int rowIndex = (position - 1) / COLUMNS;
	int colIndex = (position - 1) % COLUMNS;

	if (position >= 1 && position <= TOTAL_CELLS && board[rowIndex][colIndex] != 'X' && board[rowIndex][colIndex] != 'O')
	{
		board[rowIndex][colIndex] = player.move;
	}

	else
	{
		printBoard();
		
		if (position == 0)
			cout << "Invalid input.\nChoose a position between 1 and 9.\n";
		else if (position < 1 || position > 9)
			cout << "Not a valid position.\nChoose a position between 1 and 9.\n";
		else
			cout << "Position alredy occupied.\nChoose another position.\n";

		cout << "----------------------\n";

		playerMove(player);
	}
}

bool computerMove()
{
	for (int i = 0; i < ROWS; i++) 
	{
		for (int j = 0; j < COLUMNS; j++) 
		{
			if (board[i][j] != 'X' && board[i][j] != 'O') 
			{
				char originalValue = board[i][j];
				board[i][j] = p2.move;

				if (checkWin(p2))
				{
					p2.win--;
					return true;
				}

				board[i][j] = originalValue;
			}
		}
	}

	for (int i = 0; i < ROWS; i++) 
	{
		for (int j = 0; j < COLUMNS; j++) 
		{
			if (board[i][j] != 'X' && board[i][j] != 'O') 
			{
				char originalValue = board[i][j];
				board[i][j] = p1.move;

				if (checkWin(p2)) 
				{
					p2.win--;
					board[i][j] = p2.move;
					return true;
				}

				board[i][j] = originalValue;
			}
		}
	}

	int position;

	do 
	{
		position = rand() % TOTAL_CELLS + 1;
	} 
	while (board[(position - 1) / COLUMNS][(position - 1) % COLUMNS] == 'X' || board[(position - 1) / COLUMNS][(position - 1) % COLUMNS] == 'O');

	placeMarker(p2, position);

	return false;
}

bool checkWin(Player & player)
{
	for (int i = 0; i < ROWS; i++)
	{
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2]) ||
			(board[0][i] == board[1][i] && board[0][i] == board[2][i]))
		{
			printBoard();
			player.win++;
			showWins();
			return true;
		}
	}

	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) ||
		(board[0][2] == board[1][1] && board[0][2] == board[2][0]))
	{
		printBoard();
		player.win++;
		showWins();
		return true;
	}

	return false;
}

bool checkTie(int moveCount)
{
	if (moveCount == TOTAL_CELLS)
	{
		printBoard();
		showWins();
		return true;
	}

	return false;
}

void resetBoard()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			board[i][j] = '1' + i * COLUMNS + j;
		}
	}
}

void showWins()
{
	cout << p1.name << "'s wins: " << p1.win << "\n";
	cout << p2.name << "'s wins: " << p2.win << "\n";
	cout << "----------------------\n";
}

bool playAgain(string result)
{
	int option = 0;

	cout << HIDE_CURSOR;
	setRawMode(true);

	while (true)
	{
		printBoard();
		showWins();
		cout << result;
		printOption(option);

		int ch = getchar();

		if (ch == '\e')
		{
			ch = getchar();
			if (ch == '[')
			{
				ch = getchar();

				if (ch == 'D' && option == 1)
					option--;
				else if (ch == 'C' && option == 0) 
					option++;
			}
		} 
		else if (ch == '\n') 
		{
			break;
		} 
		else 
		{

		}
	}

	setRawMode(false);
	cout << SHOW_CURSOR;

	if (option == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}