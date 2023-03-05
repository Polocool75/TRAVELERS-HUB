# Dans ce code, la RPi ressoit l'ordre de mouvement envoyé par la Jetson Nano 

import smbus
import socket
import threading 
import time

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


def Move_Order(): # Thread 1
    recept = Unwrap(s.recvfrom(1024))
    print(recept)
    writeData("hello")
    time.sleep(50)

def LiDAR(): # Thread 2
    # - A exécuter ou bien toute les x secondes, et ou bien quand on appuie sur un bouton de la PS4 (apparait alors dans Move_Order), 
    # et/ou bien quand on est a la limite de la carte
    # - Faire tourner le LiDAR en mode sonar
    # - Envoyer les données à la Jetson  
    pass
      
      
t1 = threading.Thread(target = Move_Order) 
t2 = threading.Thread(target = LiDAR)    



      
    






    

# Plsr messagers, mais UN SEUL destinataire
