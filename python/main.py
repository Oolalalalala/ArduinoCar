import python.Bluetoobh.bluetooth as bt
import python.Bluetoobh.BTInterface as bti
from Route_Algorithm import Algorithm as ag
from collections import deque

def main():
    bti1 = bti.BTInterface()
    bti1.start()
    operations = ag.get_total_operations()
    preview = ag.get_best_route()
    m_buffer = bytes()
    route, num = 0, 0

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
                num = 0
                route += 1
                m_buffer = m_buffer[8:]
                print(uid)

if __name__ == "__main__":
    main()