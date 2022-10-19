import serial
import time

class SerialCommunication:
    def __init__(self, serial_port, baud_rate):
        self.ser = serial.Serial(serial_port, baud_rate, timeout=None)
        self.ser.reset_input_buffer()

    def wait_for_move(self):
        # Receive the move made from the Arduino in square notation ('e2e4')
        user_move = self.ser.read()
        time.sleep(1)
        bytesRemaining = self.ser.inWaiting()
        user_move += self.ser.read(bytesRemaining)
        user_move = user_move.decode('utf-8').rstrip()

        # return the move as a string for input to the chess game
        return user_move


    def send_move(self, move):
        # Send the move to the Arduino in square notation ('e5e7')
        self.ser.write(str(move).encode())

        # THE REST OF THE CODE IN THIS FUNCTION IS FOR TESTING PURPOSES ONLY
        # Wait for acknowledgement from Arduino that it received AI move
        # AI_move = self.ser.read()
        # time.sleep(1)
        # bytesRemaining = self.ser.inWaiting()
        # AI_move += self.ser.read(bytesRemaining)
        # AI_move = AI_move.decode('utf-8').rstrip()

        # print(AI_move)
        # time.sleep(2)

