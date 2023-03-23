import picamera
import pyshine as ps
import smbus
import socket
import threading 
import time
import os

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
address_slave1 = 0x11 # Adress of the slave(s)
bus = smbus.SMBus(channel) # Initialize I2C (SMBus)

s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 5005)) # 0.0.0.0 = recevoir de tout le monde

StreamProps = ps.StreamProps
StreamProps.set_Page(StreamProps,HTML)
address = ('0.0.0.0',9000) # Sur toutes les interfaces 
StreamProps.set_Mode(StreamProps,'picamera') 
server = ps.Streamer(address, StreamProps)

clear = lambda: os.system('clear')

def StringToBytes(val):
        retVal = []
        for c in val:
                retVal.append(ord(c))
        return retVal

def writeData(value, address_I2C = 0x11):
        byteValue = StringToBytes(value)
        bus.write_i2c_block_data(address_I2C,0x00,byteValue)
        return -1

def Unwrap(message):
    newMess = str(message[0])
    newMess = newMess[1:].strip("'")
    return newMess

# Les Threads 

def Move_Order(): # Thread 1
    while True:
        if connectPS4 :
            recept = Unwrap(s.recvfrom(1024))
            writeData(recept,address_slave1)
            time.sleep(200*(10**(-6)))
        else :
             time.sleep(3)

def LiDAR(): # Thread 2
    # - A exécuter ou bien toute les x secondes, et ou bien quand on appuie sur un bouton de la PS4 (apparait alors dans Move_Order), 
    # et/ou bien quand on est a la limite de la carte
    # - Faire tourner le LiDAR en mode sonar
    # - Envoyer les données à la Jetson  
    pass
      
def RetourVideo():
    global camera
    global server
    while True :
        if(fluxON) :
            with picamera.PiCamera(resolution='640x480', framerate=30) as camera:
                output = ps.StreamOut()
                StreamProps.set_Output(StreamProps,output)
                #camera.rotation = 90
                camera.start_recording(output, format='mjpeg')
                try:
                    print('Server started at','http://'+address[0]+':'+str(address[1]))
                    server.serve_forever()
                except Exception as e:
                    camera.stop_recording()
                    print(e)     
        else :
            time.sleep(3)


fluxON = False
connectPS4 = False

th_Order = threading.Thread(target = Move_Order) 
#th_LiDAR = threading.Thread(target = LiDAR)    
th_Video = threading.Thread(target = RetourVideo)

th_Order.start()
th_Video.start()


while True :
    flux_str = "Désactiver " if fluxON else "Activer "
    PS4_str = "Connecter " if not connectPS4 else "Déconnecter "
    choice = input("1. " + flux_str + "le retour vidéo\n"+
                "2. " + PS4_str + "la manette PS4\n"+
                "3. Eteindre les ESP32\n" + 
                "4. Clear Console\n")
    
    if choice == "1" :
        if fluxON :
            fluxON = False
            server.shutdown()
        else :
            fluxON = True

    elif choice == "2" :
        connectPS4 = not connectPS4

    elif choice == "3" :
        print("pas encore fonctionnel")
        #Ici on utilisera le GPIO de la Rasp dans le Vin des ESP pour les activer / desactiver
    elif choice == "4" :
        clear()
    else :
         print("Erreur")
    time.sleep(1)
    clear()