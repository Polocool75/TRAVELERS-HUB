import socket
s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 3333))
while True :
    print(s.recvfrom(1024))
    

