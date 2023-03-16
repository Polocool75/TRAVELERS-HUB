import picamera
import pyshine as ps
import smbus
import socket
import threading 
import time

HTML="""
<html>
<head>
<title>PyShine Live Streaming</title>
</head>

<body>
<center><h1> TRAVELERS</h1></center>
<center><img src="stream.mjpg" width='640' height='480' autoplay playsinline></center>
</body>
</html>
"""

channel = 1
address = 0x11 # Adress of the slave(s)
bus = smbus.SMBus(channel) # Initialize I2C (SMBus)

s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 5005)) # 0.0.0.0 = recevoir de tout le monde

def StringToBytes(val):
        retVal = []
        for c in val:
                retVal.append(ord(c))
        return retVal

def writeData(value):
        byteValue = StringToBytes(value)
        bus.write_i2c_block_data(address,0x00,byteValue)
        return -1

def Unwrap(message):
    newMess = str(message[0])
    newMess = newMess[1:].strip("'").split(",")
    return newMess

# Les Threads 

def Move_Order(): # Thread 1
    recept = Unwrap(s.recvfrom(1024))
    print(recept)
    writeData("hello")
    time.sleep(1)

def LiDAR(): # Thread 2
    # - A exécuter ou bien toute les x secondes, et ou bien quand on appuie sur un bouton de la PS4 (apparait alors dans Move_Order), 
    # et/ou bien quand on est a la limite de la carte
    # - Faire tourner le LiDAR en mode sonar
    # - Envoyer les données à la Jetson  
    pass
      
def RetourVideo():
    StreamProps = ps.StreamProps
    StreamProps.set_Page(StreamProps,HTML)
    address = ('172.21.72.236',9000) # Enter your IP address 
    StreamProps.set_Mode(StreamProps,'picamera')    
    with picamera.PiCamera(resolution='640x480', framerate=30) as camera:
        output = ps.StreamOut()
        StreamProps.set_Output(StreamProps,output)
        #camera.rotation = 90
        camera.start_recording(output, format='mjpeg')
        try:
            server = ps.Streamer(address, StreamProps)
            print('Server started at','http://'+address[0]+':'+str(address[1]))
            server.serve_forever()
        finally:
            camera.stop_recording()

def HelloWorld():
     print("Hello")
     
th_Order = threading.Thread(target = Move_Order) 
th_LiDAR = threading.Thread(target = LiDAR)    
th_Video = threading.Thread(target = RetourVideo)
th_HW = threading.Thread(target = HelloWorld)

th_Video.start()
th_HW.run()
time.sleep(10)

print("fini")

      
# Plsr messagers, mais UN SEUL destinataire
