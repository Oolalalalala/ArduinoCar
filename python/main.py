import bluetooth as bt
import BTInterface as bti
import Algorithm as ag

def main():
    bti1 = bti.BTInterface()
    bti1.start()
    operations = ag.get_total_operations()
    message=['0',0,0,'3','4','5','6','7']
    route=0
    num=0

    while True:
        start=0
        waiting=0
        waiting=bti1.getmessegecount()
        #print("a")
        if waiting>1:
            start=1
            i=0
            while start==1:
                waiting=bti1.getmessegecount()
                m=bti1.read()
                j=0
                #print("b")
                while j <waiting-1:
                    message[i]=m[j]
                    i+=1
                    j+=1
                if message[3]=='>' or message[7]=='>':
                    if message[1]=='1':
                        dir=operations[route,num]
                        bti1.send_action(dir)
                        num+=1
                        print("node")

                    elif message[1] == '2':
                        
                        uid = hex(int.from_bytes(rv, byteorder="big", signed=False))
                        num=0
                        route+=1
                        print("UID")
                    message=['0',0,0,'3','4','5','6','7']
                    start=0
            
            
            


        """
        if start == 1:
            print("c/n")
            #message = bti1.read()
            if message == 1: #node
                #dir=operations[route,num]
                #bti1.send_action(dir)
                num+=1
                print("send/n")
            elif message == 2: #RFID
                UID = bti1.get_UID()
                print("UID")
                num=0
                route+=1
        
        """ 
        



if __name__ == "__main__":
    main()