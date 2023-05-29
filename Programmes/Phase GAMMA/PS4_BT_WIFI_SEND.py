from pyPS4Controller.controller import Controller
import socket
import threading
import time
import math

UDP_IP = "10.3.141.1" # "127.0.0.1" = loopback (pour ce l'envoyer à nous même), par ailleurs on ne peut pas broadcast
UDP_PORT = 5005
MESSAGE = ""
valL,valR = 0,0
oldL, oldR = 0,0

forwardL = True
forwardR = True

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

MAX = 32767
dead = 4000
delay = 30*(10)**(-6)


def Mapping(val, oldMax = MAX, newMax = 255) :
    return int(val * (newMax/oldMax))

class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)

    def on_x_press(self):
        pass

    def on_triangle_press(self):
        pass

    def on_circle_press(self):
        pass

    def on_square_press(self):
        global MESSAGE
        MESSAGE = "B,1"
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)

    def on_L1_press(self):
        pass

    def on_L2_press(self, value):
        print("on_L2_press: {}".format(value))

    def on_R1_press(self):
        pass

    def on_R2_press(self, value):
        print("on_R2_press: {}".format(value))

    def on_L3_up(self, value):
        if abs(value) >= dead :
            global valL,forwardL
            forwardL=True
            valL= abs(Mapping(value))
        else :
            valL=0

    def on_L3_down(self, value):
        if abs(value) >= dead :
            global valL,forwardL
            forwardL = False
            valL = abs(Mapping(value))
        else :
            valL=0

    def on_L3_y_at_rest(self):
        global valL
        valL=0

    def on_R3_up(self, value):
        if abs(value) >= dead :
            global valR,forwardR
            forwardR = True
            valR = abs(Mapping(value))
        else :
            valR=0

    def on_R3_down(self, value):
        if abs(value) >= dead :
            global valR,forwardR
            forwardR = False
            valR = abs(Mapping(value))
        else :
            valR=0

    def on_R3_y_at_rest(self):
        global valR
        valR=0

    def on_L3_left(self, value):
        pass

    def on_L3_right(self, value):
        pass

    def on_L3_x_at_rest(self):
        pass

    def on_L3_press(self):
        """L3 joystick is clicked. This event is only detected when connecting without ds4drv"""
        print("on_L3_press")

    def on_up_arrow_press(self):
        print("on_up_arrow_press")

    def on_down_arrow_press(self):
        print("on_down_arrow_press")

    def on_left_arrow_press(self):
        print("on_left_arrow_press")

    def on_right_arrow_press(self):
        print("on_right_arrow_press")

    def on_R3_press(self):
        """R3 joystick is clicked. This event is only detected when connecting without ds4drv"""
        print("on_R3_press")

    def on_options_press(self):
        print("on_options_press")

    def on_share_press(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_share_press")

    def on_playstation_button_press(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_playstation_button_press")

    def on_playstation_button_release(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_playstation_button_release")
     

    #Fonctions un peu inutiles
    def on_x_release(self):
        pass

    def on_triangle_release(self):
        pass

    def on_circle_release(self):
        pass 

    def on_square_release(self):
        pass

    def on_L1_release(self):
        pass

    def on_L2_release(self):
        pass

    def on_R2_release(self):
        pass

    def on_R1_release(self):
        pass

    def on_up_down_arrow_release(self):
        pass

    def on_left_right_arrow_release(self):
        pass

    def on_L3_release(self):
        pass

    def on_R3_left(self, value):
        pass

    def on_R3_right(self, value):
        pass

    def on_R3_x_at_rest(self):
        pass

    def on_options_release(self):
        pass

    def on_R3_release(self):
        pass

    def on_share_release(self):
        pass

def InputGet():
    controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
    controller.listen()

th_input = threading.Thread(target = InputGet) 
th_input.start()

while True:
    if oldL != valL or oldR!=valR :
        MESSAGE = str("M,")+str(valL)+","+ (str("A,") if forwardL else str("R,"))+ str(valR) + (str(",A") if forwardR else str(",R"))
        oldL = valL
        oldR = valR
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
    time.sleep(delay)