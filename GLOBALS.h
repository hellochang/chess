// Todo's and Concerns

// Lv4 [maybe not]

// Graphic Interface

// ./chess --help flag?

// More tests

#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

const set<char> WHITEPIECESET{'R', 'N', 'B', 'Q', 'K', 'B', 'K', 'R', 'P'};
const set<char> BLACKPIECESET{'r', 'n', 'b', 'q', 'k', 'b', 'k', 'r', 'p'};
const set<string> VALIDPLAYERS{"human", "computer1",
                               "computer2", "computer3", "compouter4"};
const set<string> VALIDSETUPOPS{"+", "-", "=", "done"};

// Coordinates defined in the first coordinate
// cellIdx = (row - 1) * 8 + (col - 1)
// row = CellIndex / 8 + 1
// col = CellIndex % 8 + 1

// Converts index to row number
int toRow(int Idx);

// Converts index to column number
int toCol(int Idx);

// Converts row and column number to cells index
int toIdx(int row, int col);

// Converts chess coordinate to cells index
bool coordToIdx(string coord, int &Idx);

// Checks validity of coordinate
bool validCoord(string coord);

// Gets words from command line
bool getWords(vector<string> &words);

// Produce a invalid input warnin [todo (Improve)]
void badInput();

#endif

