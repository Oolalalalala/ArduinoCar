import Bluetooth.BTInterface as bti
from Route_Algorithm import Algorithm as ag
#from score import*
#import time
import logging
import score as sc
    

def main():
    
    log = logging.getLogger("scoreboard")
    logging.basicConfig(level=logging.DEBUG)
    scoreboard=sc.ScoreboardServer("ggggggggggggg", host=f"http://140.112.175.18:5000")

    score, time_remaining = scoreboard.add_UID("00000000")
    while time_remaining<80.5:


        operations = ag.get_total_operations()
        preview = ag.get_best_route()
        m_buffer = bytes()
        route, num = 0, 0

        print(preview)
        print(operations)
        
        while True:  
            if m_buffer and m_buffer[-1] == 62:
                if m_buffer[1] == 2:
                    uid = hex(int.from_bytes(m_buffer[3:7], byteorder="little", signed=False))

                    score, time_remaining = scoreboard.add_UID(uid)
                    current_score = scoreboard.get_current_score()
                    log.info(f"Current score: {current_score}")
                    
                    num = 0
                    route += 1
                    m_buffer = m_buffer[8:]
                    print(uid)

if __name__ == "__main__":
    main()