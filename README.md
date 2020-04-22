# Chess

The chess game.

## Extra Features

You may regret and undo as many of your moves as you want.

## Compile & Run

### Compile

```
make
```

### Run

```
./chess
```
The above commad starts a chess game that has a text display.
For graphic interface, execute with flag --graphic
```
./chess --graphic
```

## Usage

When the program is running, you could either start a game, configure the board before starting, or quit the game.

All coordinates must be valid coordinates on a standard chess board, among a1 - h8.

All pieces must be valid. (eg R, B, k, p ... where R is for white Rook, k is for Black King) 

You can always type invalid input towards this program, it won't crash, just that you have to re-input again.

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

to move a piece as human.

You have to indicate a valid piece to promote if needed.

```
move coordinate coordinate piece
```

For human players, you can input

```
resign
```

to give up this game. (Thus opponent wins.)

Input

```
move
```

for computer players to take a move.

Anytime you want to switch diffculty level duing the game, input

```
setlevel lv
```
lv must be a number between 1 and 4 inclusively.

You can try to set the level for human player, but only to get a warning.

Ass a human player, you can also regret, simply input

```
regret
```
After each move, the program will print the current board to tty and show who is the next player.

## Configure the board

Before a game started, or after a game finished, you can setup the board.

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

When you finish setting up, input

```
done
```

to quit setup mode.

Notice that you are only allowed to quite setup mode when the board is valid.

## Quit

Whether you are in the middle of a chess game, finishing a chess game or setting up the chess board, you can always quit the program by sending EOF signal if you wish

```
Ctrl + D
```
If you are in a game, the game will be abandoned (no score for both sides). 

Before the program exits, it will print the final scores for both sides.
