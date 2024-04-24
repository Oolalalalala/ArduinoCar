import sys
import threading
import python.Bluetoobh.bluetooth as BT


def read():
    while True:
        if bt.waiting():
            print(bt.readString())

if __name__ == "__main__":
    bt = BT.bluetooth("COM3")
    while not bt.is_open():
        pass
    print("BT Connected!")

    readThread = threading.Thread(target=read)
    readThread.daemon = True
    readThread.start()

    while True:
        msgWrite = input()
        if msgWrite == "exit":
            sys.exit()
        if '0' <= msgWrite[0] and msgWrite[0] <= '9':
            bt.write(chr(ord(msgWrite[0]) - ord('0')))