# ♗ chs

> Play chess against the Stockfish engine on the Smart Chess Board with the user interface hosted in the terminal.

<img src="https://user-images.githubusercontent.com/10540865/119232802-80c34700-baf4-11eb-9fed-af558575ae4e.png" />

## Usage

To play against the default level 1 (easiest) version of the Stockfish engine, just run `python3 chs.py` program.

### How to start playing

```
$ python3 chs.py
```

To see all possible options, use the help command.

```
$ python3 chs.py help
```

To play as the black pieces, use the `--play-black` flag.

```
$ python3 chs.py --play-black
```

You can also specify the level of the engine if you want to tweak the difficulty.

```
$ python3 chs.py --level=8
```

### How to play

There are a few things you can do while playing:

- Make moves using valid algebraic notation (e.g. `Nf3`, `e4`, etc.).
- Take back your last move by playing `back` instead of a valid move. (not supported on physical chessboard)
- Get a hint from the engine by playing `hint` instead of a valid move.

