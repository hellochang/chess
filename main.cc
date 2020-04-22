#include "Game.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        Game game{false};
        game.startGame();
    }
    else if (argc == 2)
    {
        string flag{argv[1]};
        if (flag == "--graphic")
        {
            Game game{true};
            game.startGame();
        }
    }
    else
    {
        cerr << "Invalid args. Check README for more info." << endl;
        return -1;
    }
    return 0;
}
