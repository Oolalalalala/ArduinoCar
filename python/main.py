import Bluetooth.BTInterface as bti
from Route_Algorithm import Algorithm as ag
#from score import*
#import time
import logging
import score as sc
from collections import deque
    

def main():
    bti1 = bti.BTInterface()
    bti1.start()
    
    # operations = ag.get_total_operations()
    operations = deque(["i", "r", "i", "r", "i", "r", "i"])
    preview = ag.get_best_route()
    m_buffer = bytes()
    route, num = 0, 0

    log = logging.getLogger("scoreboard")
    logging.basicConfig(level=logging.DEBUG)

    scoreboard=sc.ScoreboardServer("Team 7", host=f"http://140.112.175.18:5000")
    score, time_remaining = scoreboard.add_UID("00000000")
    while time_remaining>80.5:
        score, time_remaining = scoreboard.add_UID("00000000")

    print(preview)
    print(operations)

    for _ in range(2):
        bti1.bt.serial_write_bytes(operations.popleft().encode("utf-8"))
    
    while True:
        m_buffer += bti1.bt.read()
            
        if m_buffer and m_buffer[-1] == 62:
            print(m_buffer)
            if m_buffer[1] == 1:
                dir = operations.popleft()
                print(dir)
                bti1.bt.serial_write_bytes(dir.encode("utf-8"))
                num += 1
                m_buffer = m_buffer[4:]
                print("Next State")
            elif m_buffer[1] == 2:
                uid = hex(int.from_bytes(m_buffer[3:7], byteorder="little", signed=False))
                print(uid)
                score, time_remaining = scoreboard.add_UID(uid[2:])
                current_score = scoreboard.get_current_score()
                log.info(f"Current score: {current_score}")
                
                num = 0
                route += 1
                m_buffer = m_buffer[8:]
                print(uid)

if __name__ == "__main__":
    main()