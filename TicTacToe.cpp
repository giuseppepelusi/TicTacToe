#include <iostream>
#include <cstdlib>
#include <termios.h>

using std::cout, std::cin, std::string, std::endl;

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
const int MIN_SCORE = -2147483648;
const int MAX_SCORE = 2147483647;
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
void printBoard();
void playerMove(Player player);
void placeMarker(Player & player, int position);
void computerMove();
int minimax(int depth, bool isMaximizing);
bool checkWin();
bool checkTie();
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
			else;
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
	string result;

	do
	{
		printBoard();
        showWins();

        Player* currentPlayer;

        if (matches % 2 == 0)
        {
            currentPlayer = &p1;
        } else
        {
            currentPlayer = &p2;
        }

        if (currentPlayer->name == COMPUTER_NAME && gameMode == 'c')
        {
            computerMove();
        }
        else
        {
            playerMove(*currentPlayer);
        }

        if (checkWin()) {
            currentPlayer->win++;
            result = currentPlayer->name + " won!!\n";
            break;
        }

        if (checkTie()) {
            result = "It's a tie.\n";
            break;
        }

        matches++;
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

		if (position < 1 || position > 9)
			cout << "Invalid input.\nChoose a position between 1 and 9.\n";
		else
			cout << "Position alredy occupied.\nChoose another position.\n";

		cout << "----------------------\n";

		playerMove(player);
	}
}

void computerMove()
{
	int bestScore = MIN_SCORE;
	int bestMove = -1;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (board[i][j] != 'X' && board[i][j] != 'O')
			{
				char originalValue = board[i][j];
				board[i][j] = p2.move;
				int score = minimax(0, false);
				board[i][j] = originalValue;
				if (score > bestScore)
				{
					bestScore = score;
					bestMove = (i * COLUMNS + j) + 1;
				}
			}
		}
	}

	placeMarker(p2, bestMove);
}

int minimax(int depth, bool isMaximizing)
{
	if (checkWin())
	{
		if (isMaximizing)
			return -1;
		else
			return 1;
	}
	if (checkTie())
	{
		return 0;
	}

	int bestScore;

	if (isMaximizing)
	{
		bestScore = MIN_SCORE;
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				if (board[i][j] != 'X' && board[i][j] != 'O')
				{
					char originalValue = board[i][j];
					board[i][j] = p2.move;
					int score = minimax(depth + 1, false);
					board[i][j] = originalValue;
					if (score > bestScore)
						bestScore = score;
				}
			}
		}
	}
	else
	{
		bestScore = MAX_SCORE;
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				if (board[i][j] != 'X' && board[i][j] != 'O')
				{
					char originalValue = board[i][j];
					board[i][j] = p1.move;
					int score = minimax(depth + 1, true);
					board[i][j] = originalValue;
					if (score < bestScore)
						bestScore = score;
				}
			}
		}
	}

	return bestScore;
}

bool checkWin()
{
	for (int i = 0; i < ROWS; i++)
	{
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2]) ||
			(board[0][i] == board[1][i] && board[0][i] == board[2][i]))
		{
			return true;
		}
	}

	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) ||
		(board[0][2] == board[1][1] && board[0][2] == board[2][0]))
	{
		return true;
	}

	return false;
}

bool checkTie()
{
	for (int i = 0; i < ROWS; i++)
	{
        for (int j = 0; j < COLUMNS; j++)
		{
            if (board[i][j] != 'X' && board[i][j] != 'O')
			{
                return false;
            }
        }
    }

    return true;
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
	cout << "-----------------------\n";
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
		if (option == 0)
			cout << "Do you want to play again? [" << INVERTED_COLORS << "Y" << RESET_COLORS << "/n]";
		else
			cout << "Do you want to play again? [Y/" << INVERTED_COLORS << "n" << RESET_COLORS << "]";

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
		else if ((ch == 'y' || ch == 'Y') && option == 1)
			option--;
		else if ((ch == 'n' || ch == 'N') && option == 0)
			option++;
		else if (ch == '\n')
		{
			break;
		}
		else;
	}

	setRawMode(false);
	cout << SHOW_CURSOR;

	if (option == 0)
		return true;
	else
		return false;
}

void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}
