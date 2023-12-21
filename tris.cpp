#include <iostream>

using std::cout, std::cin, std::string;

const int M = 3;
const int N = 3;
const int TOTAL_CELLS = M * N;
char matrix[M][N];
const string COMPUTER_NAME = "Computer";
int matches;

struct Player
{
    string Name;
    char Move;
    int Win;
};

Player p1{.Move = 'X'};
Player p2{.Move = 'O'};

void startGame(char dis);
void playervsComputer();
void namesPlayers();
void printMatrix();
void playerMove(Player player);
void placeMarker(Player & player, int pos);
bool computerMove();
bool checkWin(Player & player);
bool checkTie(int moves);
void resetMatrix();
void showWins();
void playAgain(char dis);

int main()
{
    int option;
    char dis;

    p1.Win = 0;
    p2.Win = 0;
    matches = 0;

    do
    {   
        system("clear");
        
        cout << "---------TRIS---------\n";
        cout << "Choose an option:\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Computer\n";
        cout << "3. Exit\n";
        cout << "--> ";
        cin >> option;

        switch (option)
        {
            case 1:
            {
                dis = 'p';
                p2.Name = "";
                resetMatrix();
                namesPlayers();
                startGame(dis);
                break; 
            }

            case 2:
            {
                dis = 'c';
                p2.Name = COMPUTER_NAME;
                resetMatrix();
                namesPlayers();
                startGame(dis);
                break;
            }

            case 3:
            {
                cout << "Exiting program...\n";
                exit(0);
            }

            default:
            {
                continue;
            }
        }
        break;
    }
    while (true);

    return 0;
}

void startGame(char dis)
{
    int moves = 0;
    
    do
    {
        printMatrix();
        showWins();
        moves++;

        if (matches % 2 == 0)
        {
            playerMove(p1);
            if (checkWin(p1))
                break;
        }
        else if (dis == 'p' && matches % 2 == 1)
        {
            playerMove(p2);
            if (checkWin(p2))
                break;
        }
        else
        {
            computerMove();
            if (checkWin(p2))
                break;
        }

        if (checkTie(moves))
            break;

        printMatrix();
        showWins();
        moves++;

        if (matches % 2 == 1)
        {
            playerMove(p1);
            if (checkWin(p1))
                break;
        }
        else if (dis == 'p' && matches % 2 == 0)
        {
            playerMove(p2);
            if (checkWin(p2))
                break;
        }
        else
        {
            computerMove();
            if (checkWin(p2))
                break;
        }
    }
    while (true);

    matches++;

    playAgain(dis);
}

void namesPlayers()
{
    system("clear");
    
    if (p2.Name == COMPUTER_NAME)
    {
        cout << "---------TRIS---------\n\n";
        cout << "Insert your name:\n";
        cout << "--> ";
        cin >> p1.Name;
    }

    else
    {
        cout << "---------TRIS---------\n\n";
        cout << "Insert FIRST player's name:\n";
        cout << "--> ";
        cin >> p1.Name;

        cout << "\nInsert SECOND player's name:\n";
        cout << "--> ";
        cin >> p2.Name;
    }
}

void printMatrix()
{
    system("clear");

    cout << "---------TRIS---------\n\n";

    for (int i = 0; i < M; i++)
    {
        cout << "     ";
        for (int j = 0; j < N; j++)
        {
            cout << " " << matrix[i][j] << " ";
            if (j < N - 1)
                cout << "|";
        }

        cout << "\n";

        if (i < M - 1)
        {
            cout << "     " << "-----------\n";
        }
    }

    cout << "\n";
}

void playerMove(Player player)
{
    int pos;

    cout << player.Name << "'s turn\n";
    cout << "Choose where to place your \"" << player.Move << "\": ";
    cin >> pos;

    placeMarker(player, pos);
}

void placeMarker(Player & player, int pos)
{
    int row = (pos - 1) / N;
    int col = (pos - 1) % N;

    if (pos >= 1 && pos <= TOTAL_CELLS && matrix[row][col] != 'X' && matrix[row][col] != 'O')
    {
        matrix[row][col] = player.Move;
    }

    else
    {
        printMatrix();
        
        if (pos < 1 || pos > 9)
            cout << "Not a valid position.\nChoose a position between 1 and 9.\n";

        else
            cout << "Position alredy occupied.\nChoose another position.\n";

        cout << "----------------------\n";
        playerMove(player);
    }
}

bool computerMove()
{
    for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            if (matrix[i][j] != 'X' && matrix[i][j] != 'O') 
            {
                char original_value = matrix[i][j];
                matrix[i][j] = p2.Move;

                if (checkWin(p2))
                {
                    p2.Win--;
                    return true;
                }

                matrix[i][j] = original_value;
            }
        }
    }

    for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            if (matrix[i][j] != 'X' && matrix[i][j] != 'O') 
            {
                char original_value = matrix[i][j];
                matrix[i][j] = p1.Move;

                if (checkWin(p2)) 
                {
                    p2.Win--;
                    matrix[i][j] = p2.Move;
                    return true;
                }

                matrix[i][j] = original_value;
            }
        }
    }

    int pos;

    do 
    {
        pos = rand() % TOTAL_CELLS + 1;
    } 
    while (matrix[(pos - 1) / N][(pos - 1) % N] == 'X' || matrix[(pos - 1) / N][(pos - 1) % N] == 'O');

    placeMarker(p2, pos);

    return false;
}

bool checkWin(Player & player)
{
    for (int i = 0; i < M; i++)
    {
        if ((matrix[i][0] == matrix[i][1] && matrix[i][0] == matrix[i][2]) ||
            (matrix[0][i] == matrix[1][i] && matrix[0][i] == matrix[2][i]))
        {
            printMatrix();
            player.Win++;
            showWins();
            cout << player.Name << " won!!\n";
            return true;
        }
    }

    if ((matrix[0][0] == matrix[1][1] && matrix[0][0] == matrix[2][2]) ||
        (matrix[0][2] == matrix[1][1] && matrix[0][2] == matrix[2][0]))
    {
        printMatrix();
        player.Win++;
        showWins();
        cout << player.Name << " won!!\n";
        return true;
    }

    return false;
}

bool checkTie(int moves)
{
    if (moves == TOTAL_CELLS)
    {
        printMatrix();
        showWins();
        cout << "It's a tie.\n";
        return true;
    }

    return false;
}

void resetMatrix()
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = '1' + i * N + j;
        }
    }
}

void showWins()
{
    cout << p1.Name << "'s wins: " << p1.Win << "\n";
    cout << p2.Name << "'s wins: " << p2.Win << "\n";
    cout << "----------------------\n";
}

void playAgain(char dis)
{
    char option;
    
    do
    {
        cout << "Do you want to play again? (y/n): ";
        cin >> option;
    
        switch (option)
        {
            case 'y':
            case 'Y':
            {
                resetMatrix();
                startGame(dis);
                break;
            }

            case 'n':
            case 'N':
            {
                main();
                break;
            }
            
            default:
            {
                printMatrix();
                showWins();
                cout << "Not a valid option.\n";
            }
        }
    }
    while (option != 'n' && option != 'N');
}