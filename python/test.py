import socket
import struct
import sys
import serial


localIP = ""
localPort = 20005
bufferSize = 1024

dataDict = {}
#arduino = serial.Serial('/dev/ttyACM0', 9600)
pedal = serial.Serial('COM18', 115200, timeout=1)

with open("dataformat.csv", "r") as dataformatfile:
    array = []
    offset = 0;

    for line in dataformatfile:
        newdata = {}

        data = line.split(' ')
        type = data[0][:1]
        tmp = data[1].split(';')
        name = tmp[0]
        newoffset = offset
        if (type == 'i') : # signed integer
            newlength = 4
            offset = offset + newlength
        elif (type == 'I') : # unsigned integer
            newlength = 4
            offset = offset + newlength
        elif (type == 'f') : # 32-bit float
            newlength = 4
            offset = offset + newlength
        elif (type == 'H') : # 16-bit unsigned integer
            newlength = 2
            offset = offset + newlength
        elif (type == 'B') : # 8-bit unsigned integer
            newlength = 1
            offset = offset + newlength
        else :
            print("error in keys",data )
            sys.exit(1)
        #print(type,name)
        newdata["type"] = type
        newdata["offset"] = newoffset
        newdata["length"] = newlength
        #newdata["value"] = 0
        dataDict[name] = newdata
        #print(dataDict)
        array.append(line)

timer = 0
lastSend = 0
broms = 100
def sendUpdate():
    global broms
    broms -=1
    if broms>100:
        broms = 100
    if broms <5:
        broms = 5
    text = f"b{broms};"
    pedal.write(text.encode())
    print("send", text)
#print(array)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', localPort))

running = True
while running:
    message, address = s.recvfrom(1024)
    timer +=1
    for key,item in dataDict.items():
        (value,) = struct.unpack_from(item["type"],message, offset=item["offset"])
        item["value"] = value


    for key,item in dataDict.items():
        #print(key, item["value"] )
        continue

    speed = dataDict["VelocityZ"]["value"]
    if speed >12.0:

        fl = dataDict["WheelRotationSpeedFrontLeft"]["value"]
        fr = dataDict["WheelRotationSpeedFrontRight"]["value"]
        rl = dataDict["WheelRotationSpeedRearLeft"]["value"]
        rr = dataDict["WheelRotationSpeedRearRight"]["value"]
        if fl < 1.0 or fr < 1.0 or rl < 1.0 or rr < 1.0:

            print(speed, fl, fr, rl, rr)
            if (lastSend < timer):
                sendUpdate()
                lastSend = timer + 100
    while pedal.inWaiting():
        res2 = pedal.read().decode()
        print(res2, end='')
