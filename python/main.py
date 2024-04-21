import bluetooth as bt
import BTInterface as bti
import Route_Algorithm.Algorithm as ag

def main():
    route=0
    num=0
    bti1 = bti.BTInterface()
    bti1.start()
    while True:
        #bt1 = bt.Bluetooth()
        
        #operations = ag.get_total_operations()
        start=0
        startbool = bti1.read()
        print(startbool)
        if startbool=='<':
            start=1
            print("sssssend/n")
        elif startbool=='>':
            start=0
            print("sennnnnnnnd/n")

        
        if start == 1:
            print("seeeeeeeeend/n")
            message = bti1.read()
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

        break



if __name__ == "__main__":
    main()