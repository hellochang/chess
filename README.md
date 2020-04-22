# Chess

The classic chess game.

## Extra Features

You may regret and undo as many of your moves as you want.

You can dynamically set the difficulty level for Computer Players.

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

While the program is running, you could either start a game, configure the board before starting, or quit the game.

All coordinates must be valid coordinates on a standard chess board, among a1 - h8.

All pieces must be valid. (eg R, B, k, p ... where R is for white Rook, k is for Black King) 

You can always type invalid input towards this program. The program won't crash, instead, an error message is displayed and you may another input again.

## Start a game

```
game PLAYER1 PLAYER2
```

PLAYER must be valid, and among one of the following options

```
{human | computer1 | computer2 | computer3 | computer4}
```

Note: computer1 is the easiest level and computer4 is the most difficult.

After a game is started, type input

```
move COORDINATE COORDINATE
```

to move a piece as human.
And type input

```
move
```
for computer players to take a move.

You may indicate a valid piece to promote if needed.

```
move COORDINATE COORDINATE PIECE
```

For human players, you can input

```
resign
```

to give up this game. (Thus opponent wins.)


Anytime you want to switch diffculty level during the game, input

```
setlevel LV
```
LV must be a number between 1 and 4 inclusive.

You may try to set the level for human player, but you'll only get a warning.

As a human player, you may also regret (i.e. undo your previous move YAY!), simply input

```
regret
```
After each move, the program will print the current board and indicate who is the next player.

## Configure the board

Before a game start or after another game finish, you can customize the board set up.

To add a piece, use the commad

```
+ PIECE COORDINATE
```

To delete a piece

```
- COORDINATE
```

To set which color goes first

```
= COLOR
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
If you are in the middle of a game, the game will be abandoned (no score for both sides). 

Before the program exits, the program will print the final scores for both colors.
