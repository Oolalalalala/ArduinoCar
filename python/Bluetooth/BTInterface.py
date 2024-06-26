import logging
from typing import Optional
from .bluetooth import *

log = logging.getLogger(__name__)

# hint: You may design additional functions to execute the input command,
# which will be helpful when debugging :)


class BTInterface:

    def __init__(self, port: Optional[str] = None):
        log.info("Arduino Bluetooth Connect Program.")
        self.bt = Bluetooth()
        if port is None:
            port = input("PC bluetooth port name: ")
        while not self.bt.do_connect(port):
            if port == "quit":
                self.bt.disconnect()
                quit()
            port = input("PC bluetooth port name: ")

    def start(self):
        input("Press enter to start.")
        #self.bt.serial_write_string("s")

    def get_UID(self):
        return self.bt.serial_read_byte()

    # def send_action(self, dirc):
    #     # send the action to car
    #     self.bt.serial_write_bytes(self, dirc)
    #     #return

    def end_process(self):
        self.bt.serial_write_string("e")
        self.bt.disconnect()

    # def read(self):
    #     return self.bt.read()
    
    def getmessagecount(self):
        return self.bt.serial.in_waiting
        


if __name__ == "__main__":
    test = BTInterface()
    test.start()
    test.end_process()