
import chess
import editdistance
from time import sleep

from chs.client.ending import GameOver
from chs.engine.parser import FenParser
from chs.engine.stockfish import Engine
from chs.ui.board import Board
from chs.utils.core import Colors, Styles


class GameOverException(Exception):
  pass

class WhiteWinsException(GameOverException):
  pass

class BlackWinsException(GameOverException):
  pass

class DrawException(GameOverException):
  pass

class ResignException(GameOverException):
  pass

class Client(object):
  BACK = 'back'
  HINT = 'hint'

  def __init__(self, level, play_as):
    self.ui_board = Board(level, play_as)
    self.play_as = play_as
    self.board = chess.Board()
    self.parser = FenParser(self.board.fen())
    self.engine = Engine(level)  # Engine you're playing against.
    self.hint_engine = Engine(8)  # Engine used to help give you hints.
    self.board.san_move_stack_white = []
    self.board.san_move_stack_black = []
    self.board.help_engine_hint = None

  def run(self):
    try:
      while True:
        self.check_game_over()
        if self.is_user_move():
          self.make_turn()
        else:
          self.computer_turn()
    except BlackWinsException:
      self.ui_board.generate(self.fen(), self.board, self.engine, GameOver.BLACK_WINS)
    except WhiteWinsException:
      self.ui_board.generate(self.fen(), self.board, self.engine, GameOver.WHITE_WINS)
    except DrawException:
      self.ui_board.generate(self.fen(), self.board, self.engine, GameOver.DRAW)
    except ResignException:
      self.ui_board.generate(self.fen(), self.board, self.engine, GameOver.RESIGN)
    finally:
      self.engine.done()
      self.hint_engine.done()

  def check_game_over(self):
    if self.board.is_game_over():
      result = self.board.result()
      if result == '1-0':
        raise WhiteWinsException
      if result == '0-1':
        raise BlackWinsException
      if result == '1/2-1/2':
        raise DrawException

  def moves(self):
    return map(self.board.san, self.board.legal_moves)

  def closest_move(self, illegal_move):
    for move in self.moves():
      distance = editdistance.eval(move, illegal_move)
      if distance <= 1:
        return move
    return None

  def make_turn(self, meta=(False, None)):
    (failed, prev_move) = meta
    self.ui_board.generate(self.fen(), self.board, self.engine)
    if failed:
      if prev_move == self.BACK:
        print('{}{}  ⃠ You cannot go back, no moves were made.{}'.format(
          Styles.PADDING_SMALL, Colors.RED, Colors.RESET
        ))
      else:
        maybe_move = self.closest_move(prev_move)
        if maybe_move is not None:
          error_string = '{}{}  ⃠ Illegal, did you mean {}{}{}{}{}'.format(
            Colors.RED, Styles.PADDING_SMALL,\
            Colors.WHITE, Colors.UNDERLINE, Colors.BOLD, maybe_move, Colors.RESET
          )
        else:
          error_string = '{}{}  ⃠ Illegal, try again.'.format(Styles.PADDING_SMALL, Colors.RED)
        print(error_string)
    else:
      print('')
    try:

      print('{}{}{}┏━ Your move ━━━━━━━━━━━━━━━━━━━━━┓ \n{}┗{}{}{}waiting for user move on board{}'.format(
        Styles.PADDING_SMALL, Colors.WHITE, Colors.BOLD,\
        Styles.PADDING_SMALL, Styles.PADDING_SMALL, Colors.RESET, Colors.GRAY, Colors.RESET)
      )

      # this is where we need to input our move from the Arduino

      # write a function that was able to accomplish what we did in the lab between Arduino and Raspberry Pi
      # waits until it gets input from the Arduino and then sets that input equal to move variable

      # add a button on the board that indicates we want a hint
      # all that needs to be done is send the string 'hint'
      # if we send the string 'hint' then send the Arduino acknowledgement that we have received, but do nothing so logic stays same in Arduino code
      # if we want, we can try and send back the move recommended by the engine, but all the help stuff is after we get initial comm going

      # think about what to do if illegal move, will change logic a bit but also down the road after initial comms

      # adding a back button would be hard to do with the automatic moves, so maybe leave that out

      move = input()

      self.ui_board.generate(self.fen(), self.board, self.engine)


      print('\n{}{}{}┏━ Your move ━━━━━━━━━━━┓ \n{}┗{}{}'.format(
        Styles.PADDING_SMALL, Colors.WHITE, Colors.BOLD,\
        Styles.PADDING_SMALL, Styles.PADDING_SMALL, Colors.RESET), move)

      sleep(2)

      if move == self.BACK:
        self.board.pop()
        self.board.pop()
      elif move == self.HINT:
        hint = self.hint_engine.play(self.board, 1.000)
        self.board.help_engine_hint = self.board.uci(hint.move)
      else:
        s = self.board.parse_san(move)
        if self.play_as == chess.WHITE:
          self.board.san_move_stack_white.append(self.board.san(s))
        else:
          self.board.san_move_stack_black.append(self.board.san(s))
        self.board.push_san(move)
        self.board.help_engine_hint = None  # Reset hint if you've made your move.
    except ValueError:
      self.board.help_engine_hint = None  # Reset hint if you wanna dismiss it by invalid moving.
      self.make_turn((True, move))
    except IndexError:
      self.make_turn((True, move))
    except:
      raise ResignException

  def computer_turn(self):
    self.ui_board.generate(self.fen(), self.board, self.engine)
    print('\n{}{}{}┏━ Opponent\'s move ━━━━━┓ \n{}┗{}{}{}thinking...{}'.format(
      Styles.PADDING_SMALL, Colors.WHITE, Colors.BOLD,\
      Styles.PADDING_SMALL, Styles.PADDING_SMALL, Colors.RESET, Colors.GRAY, Colors.RESET)
    )
    result = self.engine.play(self.board)
    if self.play_as == chess.WHITE:
      self.board.san_move_stack_black.append(self.board.san(result.move))
    else:
      self.board.san_move_stack_white.append(self.board.san(result.move))
    self.board.push(result.move)

  def fen(self):
    return self.board.fen()

  def is_user_move(self):
    to_move = self.parser.get_to_move(self.fen())
    if self.play_as == chess.WHITE:
      return to_move == 'w'
    else:
      return to_move == 'b'
