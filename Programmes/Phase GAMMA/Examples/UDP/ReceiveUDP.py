import socket

def Unwrap(message):
    newMess = str(message[0])
    newMess = newMess[1:].strip("'").split(",")
    return newMess
    
s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 5005)) # 0.0.0.0 = recevoir de tout le monde
# Plsr messagers, mais UN SEUL destinataire
while True :
    recept = Unwrap(s.recvfrom(1024))
    print(recept)