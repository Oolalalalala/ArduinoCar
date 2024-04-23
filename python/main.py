import bluetooth as bt
import BTInterface as bti
from Route_Algorithm import Algorithm as ag
def main():
    bti1 = bti.BTInterface()
    bti1.start()
    operations = ag.get_total_operations()
    messege=['0',0,0,'3','4','5','6','7']
    a=['a','b','c','d']
    route=0
    num=0

    while True:
        start=0
        waiting=0
        waiting=bti1.getmessegecount()
        if waiting>1:
            start=1
            i=0
            while start==1:
                waiting=bti1.getmessegecount()
                m=bti1.read()
                j=0
                while j <waiting-1:
                    messege[i]=m[j]
                    i+=1
                    j+=1
                if messege[3]==b'>' or messege[7]==b'>':
                    if messege[1]==b'0x01':
                        dir=operations[route,num]
                        bti1.send_action(dir)
                        num+=1
                        print("node")

                    elif messege[1] == b'0x2':
                        for k in range(4):
                            a[k]=messege[k+3]
                        uid = hex(int.from_bytes(a, byteorder="big", signed=False))
                        num=0
                        route+=1
                        print("UID")
                    messege=['0',0,0,'3','4','5','6','7']
                    start=0

if __name__ == "__main__":
    main()