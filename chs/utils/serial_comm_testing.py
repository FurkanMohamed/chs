import serial
import time

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=None)
    ser.reset_input_buffer()
    counter = 0

    print('Waiting for input from Arduino...')

    while True:
        # wait for Arduino to send us "Button Pressed"
        # really this would be a game board move in the real program like 'a1a2'
        tdata = ser.read()
        time.sleep(1)
        bytesRemaining = ser.inWaiting()
        tdata += ser.read(bytesRemaining)
        tdata = tdata.decode('utf-8').rstrip()

        # instead of printing, we send the move to the AI
        print(tdata)

        # we need to get the move from the AI instead of hardcoded
        moves = ['g1g1', 'f1f5', 'g1h3', 'a1a2', 'b1b2']

        # send Arduino the move that we made in 'a1a2 format
        # may need to write a conversion function if AI provides us with algebraic notation
        ser.write(str(moves[counter]).encode())
        counter += 1

        # Wait for acknowledgement from Arduino that it received AI move
        tdata = ser.read()
        time.sleep(1)
        bytesRemaining = ser.inWaiting()
        tdata += ser.read(bytesRemaining)
        tdata = tdata.decode('utf-8').rstrip()

        print(tdata)
