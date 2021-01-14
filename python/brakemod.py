import serial

broms = 100
ratt = serial.Serial('COM16', 115200, timeout=1)
pedal = serial.Serial('COM18', 115200, timeout=1)
print("started")
while(True):
    while ratt.inWaiting():
        res = ratt.read().decode()
        if (res == 's'):
            res = ratt.read().decode()
            if (res == 'b'):
                res = ratt.read().decode()
                if (res == '+'):
                    broms += 5
                    
                if (res == '-'):
                    broms -= 5
                if broms>100:
                    broms = 100
                if broms <5:
                    broms = 5
                text = f"b{broms};"
                pedal.write(text.encode())
                print("send", text)
        #print("startbyte")
    #print(res)
    while pedal.inWaiting():
        res2 = pedal.read().decode()
        print(res2, end='')
    