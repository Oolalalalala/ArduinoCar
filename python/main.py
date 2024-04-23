import bluetooth as bt
import BTInterface as bti
from Route_Algorithm import Algorithm as ag
def main():
    bti1 = bti.BTInterface()
    bti1.start()
    #operations = ag.get_total_operations()
    message=['0',0,0,'3','4','5','6','7']
    a=['a','b','c','d']
    route=0
    num=0

    while True:
        start=0
        waiting=0
        waiting=bti1.getmessagecount()
        
        if waiting>1:
            start=1
            i=0
            print("aaaaaaaaaaaaaaaaaaaaa")
            while start==1:
                waiting = bti1.getmessagecount()
                m = bti1.bt.read()
                j = 0
               
                while j < waiting-1:
                    message[i]=m[j]
                    i+=1
                    j+=1
                if message[3]==b'>' or message[7]==b'>':
                    if message[1]==b'0x01':
#                        dir=operations[route,num]
                        bti1.send_action(dir)
                        num+=1
                        print("node")

                    elif message[1] == b'0x2':
                        for k in range(4):
                            a[k]=message[k+3]
                        uid = hex(int.from_bytes(a, byteorder="big", signed=False))
                        num=0
                        route+=1
                        print("UID")
                    message=['0',0,0,'3','4','5','6','7']
                    start=0

if __name__ == "__main__":
    main()