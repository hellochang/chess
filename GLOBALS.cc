#include "GLOBALS.h"

int toRow(int Idx)
{
    return Idx / 8 + 1;
}

int toCol(int Idx)
{
    return Idx % 8 + 1;
}

int toIdx(int row, int col)
{
    return (row - 1) * 8 + (col - 1);
}

bool validCoord(string coord)
{
    return coord.size() == 2 &&
           'a' <= coord[0] && coord[0] <= 'h' &&
           '1' <= coord[1] && coord[1] <= '8';
}

bool coordToIdx(string coord, int &idx)
{
    // a-h & 1-8
    if (validCoord(coord))
    {
        idx = toIdx(coord[1] - '0', coord[0] - 'a' + 1);
        return true;
    }
    return false;
}

bool getWords(vector<string> &words)
{
    // Gets a line and breaks it into words
    words.clear();
    string line;
    if (!getline(cin, line))
        return false;
    stringstream ss{line};
    string word;
    while (ss >> word)
        words.push_back(word);
    return true;
}

void badInput()
{
    cout << "Invalid input! Please try again!" << endl;
}