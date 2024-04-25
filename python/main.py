import Bluetooth.BTInterface as bti
from Route_Algorithm import Algorithm as ag
import logging
import score as sc
from collections import deque
    

def main():
    bti1 = bti.BTInterface()
    bti1.start()

    operations = ag.get_total_operations()
    preview = ag.get_best_route()
    m_buffer = bytes()

    log = logging.getLogger("scoreboard")
    logging.basicConfig(level=logging.DEBUG)

    scoreboard=sc.ScoreboardServer("chichonlo", host=f"http://140.112.175.18:5000")
    time_remaining = 81
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
                outdir = "l" if dir == "L" else dir
                print(outdir)
                bti1.bt.serial_write_bytes(outdir.encode("utf-8"))
                m_buffer = m_buffer[4:]
                print("Next State")
            elif m_buffer[1] == 2:
                uid = hex(int.from_bytes(m_buffer[3:7], byteorder="little", signed=False))
                print(uid)
                score, time_remaining = scoreboard.add_UID(uid[2:])
                current_score = scoreboard.get_current_score()
                log.info(f"Current score: {current_score}")
                m_buffer = m_buffer[8:]
                print(uid)

            elif m_buffer[1] == 3:
                if dir=='r':
                    fixdir='l'
                    print(fixdir)
                    bti1.bt.serial_write_bytes(fixdir.encode("utf-8"))
                elif dir=='L' or (dir=='l' and operations[0]=='l'):
                    fixdir = operations.popleft()
                    print(fixdir)
                    bti1.bt.serial_write_bytes(fixdir.encode("utf-8"))
                elif dir=='l':
                    fixdir='r'
                    print(fixdir)
                    bti1.bt.serial_write_bytes(fixdir.encode("utf-8"))
                m_buffer = m_buffer[4:]

if __name__ == "__main__":
    main()