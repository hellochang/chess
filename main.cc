#include "Game.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        Game game;
        game.startGame();
    }
    else if (argc == 2 && string{argv[1]} == "--graphic")
    {
        Game game;
        game.setGraphic(true);
        game.startGame();
    }
    else
    {
        cerr << "Invalid args. Check README for more info." << endl;
        return -1;
    }
    return 0;
}
