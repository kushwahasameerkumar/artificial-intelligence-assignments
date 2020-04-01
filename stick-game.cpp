#include <bits/stdc++.h>

using namespace std;

class Game
{
private:
    int n;

public:
    vector<int> memoization, choices; // for memoization
    Game(int n)
    {
        this->n = n;
        memoization.resize(n + 1, INT_MIN);
        choices.resize(n + 1, INT_MIN);
    }
    void playAIHuman();
    void playAIAI();
    //returns choice of AI
    int AIturn(int n);
    //return 1 if maximizing player(A) wins, 0 otherwise
    int minmax(int n, bool isMaximizing, int alpha, int beta);
};

int Game::minmax(int n, bool maximizing, int alpha, int beta)
{
    //return on leaf node
    if (n == 1)
    {
        if (maximizing)
            return 0;
        else
            return 1;
    }

    if (maximizing)
    {
        //check if memoized
        if (this->memoization[n] > INT_MIN)
        {
            return this->memoization[n];
        }

        //maximizing player
        int winning = INT_MIN;
        for (int i = 1; i <= 3 && n - i > 0; ++i)
        {
            int winningA = minmax(n - i, false, alpha, beta);
            winning = max(winningA, winning);
            alpha = max(alpha, winningA);

            if (winningA == 1)
            {
                this->choices[n] = i;
                this->memoization[n] = winning; //memoization
                return winning;
            }

            if (alpha >= beta)
                break;
        }

        return winning;
    }
    else
    {
        //minimizing player
        int winning = INT_MAX;
        for (int i = 1; i <= 3 && n - i > 0; ++i)
        {
            int winningA = minmax(n - i, true, alpha, beta);
            winning = min(winningA, winning);
            beta = min(beta, winningA);

            if (alpha >= beta)
                break;
        }

        return winning;
    }
}

int Game::AIturn(int n)
{
    int choice;

    minmax(n, true, INT_MIN, INT_MAX);

    if (this->choices[n] > INT_MIN)
        choice = this->choices[n];
    else
        choice = 1;

    return choice;
}

void Game::playAIAI()
{
    cout << "\nPlaying AI vs AI\n\n";

    bool playerA = true;

    while (n > 0)
    {
        int choice;

        minmax(n, true, INT_MIN, INT_MAX);

        if (this->choices[n] > INT_MIN)
            choice = this->choices[n];
        else
            choice = 1;

        if (playerA)
        {
            cout << "Player A: " << choice << endl;
            if (n == 1)
                cout << "\nGame Over !\nPlayer B won !!!\n";
        }
        else
        {
            cout << "Player B:\t" << choice << endl;
            if (n == 1)
                cout << "\nGame Over !\nPlayer A won !!!\n";
        }

        playerA = !playerA; //swap player
        n -= choice;
    }
}

char getPlayerName(int player)
{
    return player == 1 ? 'A' : 'B';
}

void Game::playAIHuman()
{
    cout << "\nPlaying AI vs HUMAN\n\n";

    int player = rand() % 2;
    cout << "You are playing as Player " << getPlayerName(player) << endl
         << endl;

    while (n > 0)
    {
        int choice;

        //PLAYER A is Human
        if (player == 1)
        {
            cout << "Player A: ";
            cin >> choice;
            if (n - choice < 0)
            {
                cout << "You are picking more sticks than remaining... terminating...";
                break;
            }
            n -= choice;

            if (n <= 0)
            {
                cout << "\nGame Over !\nPlayer B won !!!\n";
                break;
            }

            choice = AIturn(n);
            cout << "Player B:\t" << choice << endl;
            n -= choice;

            if (n <= 0)
            {
                cout << "\nGame Over !\nPlayer A won !!!\n";
                break;
            }
        }
        else
        {
            //Player B is Human
            choice = AIturn(n);
            cout << "Player A:\t" << choice << endl;
            n -= choice;

            if (n <= 0)
            {
                cout << "\nGame Over !\nPlayer B won !!!\n";
                break;
            }

            cout << "Player B: ";
            cin >> choice;
            if (n - choice < 0)
            {
                cout << "You are picking more sticks than remaining... terminating...";
                break;
            }
            n -= choice;

            if (n <= 0)
            {
                cout << "\nGame Over !\nPlayer A won !!!\n";
                break;
            }
        }
    }
}

int main()
{
    srand(time(0));

    int ch, n;
    cout << "Menu:\n1. AI vs Human\n2.AI vs AI\nEnter your choice: ";
    cin >> ch;

    cout << "Enter the number of sticks: ";
    cin >> n;

    Game game(n);

    if (ch == 1)
    {
        //AI vs Human
        game.playAIHuman();
    }
    else if (ch == 2)
    {
        //AI vs AI
        game.playAIAI();
    }
    else
    {
        cout << "Please choose a valid option... exiting...";
    }

    return 0;
}