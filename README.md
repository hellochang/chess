# Chess

The chess game.

## Extra Features

You can regret and undo as many of your moves as you want.

## Compile & Run

### Compile

```
make
```

### Run

```
./chess
```
The above commad will have a text display.
For graphic interface, execute with flag --graphic
```
./chess --graphic
```

## Usage

When the program is running, you can either start a game, configure the board before starting, or quit the game.

All coordinates must be valid coordinates on a standard chess board, among a1 - h8.

All pieces must be valid. (eg R, B, k, p ...)

You can always type invalid input towards this program, and it won't crash, just that you have to re-input again.

## Start a game

```
game player1 player2
```

players must be valid, among

```
{human | computer1 | computer2 | computer3 | computer4}
```

Here computer1 is the easiest level and computer4 is the hardest.

When started, input

```
move coordinate coordinate
```

to move piece as human.

You also have to indicate a valid piece to promote when necessary.

```
move coordinate coordinate piece
```

Input

```
move
```

for computer players to take a move.

Ass a human player, you can also regret, simply input

```
regret
```
After each move, the program will print the current board to tty and show who is the next player.

## Configure the board

To add a piece by

```
+ piece coordinate
```

To delete a piece

```
- coordinate
```

To set Which side goes first

```
= color
```
Where color is one of

```
{white, black}
```

## Quit

Whether you are in the middle of a chess game, finishing a chess game or setting up the chess board, you can always quit the program by sending EOF signal if you wish

```
Ctrl + D
```
If you are in a game, the game will be abandoned (no score for both sides). 

Before the program exits, it will print the final scores for both sides.
