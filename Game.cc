#include "Game.h"

// startGame() begins the game
void Game::startGame()
{
    printMenu();
    bool customizedBoard = false;

    vector<string> words;
    for (; getWords(words);)
    {
        // game
        if (words.size() == 3 && words[0] == "game" &&
            VALIDPLAYERS.count(words[1]) && VALIDPLAYERS.count(words[2]))
        {
            // If it the board has been set up, don't initialize it.
            if (!customizedBoard)
                b = Board(1);
            customizedBoard = false;
            if (words[1] == "human")
                p1 = make_shared<Human>(b, true);
            else
                p1 = make_shared<Computer>(b, true, words[1][8] - '0');
            if (words[2] == "human")
                p2 = make_shared<Human>(b, false);
            else
                p2 = make_shared<Computer>(b, false, words[2][8] - '0');
            if (!run()) // reach EOF
                break;
        }
        // setup
        else if (words.size() == 1 && words[0] == "setup")
        {
            b = Board();
            if (!setup()) // reach EOF
                break;
            else
                cout << "Setup completed." << endl;
            customizedBoard = true;
        }
        // bad input
        else
            badInput();
        printMenu();
    }
    // EOF
    printScores();
}


// run() runs game on current board
bool Game::run()
{
    p1->bindOppo(p2);
    p2->bindOppo(p1);
    saveBoard(); // save the initialized board to history
    for (;;)
    {
        b.printBoard();
        cout << "Next Player to move is: Player" << (b.isWhiteTurn ? 1 : 2) << endl;
        shared_ptr<Player> cur = b.isWhiteTurn ? p1 : p2;
        int &oppoScore = b.isWhiteTurn ? BScore : WScore;
        int &myScore = b.isWhiteTurn ? WScore : BScore;
        int res = cur->move();
        if (res < 0 || res > 3) // continue
        {
            b.isWhiteTurn = !b.isWhiteTurn;
            saveBoard();
        }
        else if (res == 0) // EOF
            return false;
        else if (res == 1)
        {
            cout << "Player" << (b.isWhiteTurn ? 2 : 1) << " wins!" << endl;
            oppoScore += 2;
            return true;
        }
        else if (res == 2)
        {
            cout << "Draw !" << endl;
            ++myScore;
            ++oppoScore;
            return true;
        }
        else
        {
            if (history.size() > 2)
            {
                b = history[history.size() - 3];
                history.pop_back();
                history.pop_back();
                cout << "Regret!" << endl;
            }
            else
            {
                cout << "You can go back no further!" << endl;
            }
        }
    }
}


// setup() sets up board
bool Game::setup()
{
    b.clear();
    vector<string> words;
    for (; getWords(words);)
    {
        if (!words.size())
            continue;
        else if (!VALIDSETUPOPS.count(words[0]))
            badInput();
        if (words[0] == "done")
        {
            if (words.size() == 1)
                if (validBoard())
                    return true;
                else
                    cout << "This is not a valid setup!" << endl;
            else
                badInput();
        }
        else if (words[0] == "+")
        {
            if (words.size() == 3 && words[1].size() == 1 &&
                (WHITEPIECESET.count(words[1][0]) || BLACKPIECESET.count(words[1][0])) &&
                validCoord(words[2]))
            {
                b.piece(words[2][1] - '0', words[2][0] - 'a' + 1) = words[1][0];
                b.printBoard();
            }
            else
                badInput();
        }
        else if (words[0] == "-")
        {
            if (words.size() == 2 && validCoord(words[1]))
            {
                b.piece(words[1][0] - 'a' + 1, words[1][1] - '0') = '\0';
                b.printBoard();
            }
            else
                badInput();
        }
        else if (words[0] == "=")
        {
            if (words.size() == 2 && (words[1] == "white" || words[1] == "black"))
            {
                b.isWhiteTurn = (words[1] == "white");
                cout << "Next Player to move is: Player" << (b.isWhiteTurn ? 1 : 2) << endl;
            }
            else
                badInput();
        }
    }
    return false;
}


// printScores() prints players' final game scores by colour
void Game::printScores()
{
    cout << "Final Score:" << endl
         << "White: " << to_string(WScore / 2) + (WScore % 2 ? ".5" : "") << endl
         << "Black: " << to_string(BScore / 2) + (BScore % 2 ? ".5" : "") << endl;
}


// validBoard() checks validity of chess board
bool Game::validBoard()
{
    // Creates two "virtual players" to check validity
    shared_ptr<Player> p1 = make_shared<Computer>(b, true);
    shared_ptr<Player> p2 = make_shared<Computer>(b, false);
    p1->bindOppo(p2);
    p2->bindOppo(p1);
    return p1->eval() && p2->eval() && !p1->inCheck() && !p2->inCheck();
}


// printMenu() prints the menu of the game
void Game::printMenu()
{
    cout << "*************************   CHESS   *************************" << endl
         << "Press Ctrl + D to quit and show scores" << endl
         << "Type game [human/computer[1-4]] [human/computer[1-4]] to play" << endl
         << "Type setup to create a board" << endl;
}


// saveBoard() saves the chess Board for possible future undos.
void Game::saveBoard()
{
    history.push_back(b);
}
