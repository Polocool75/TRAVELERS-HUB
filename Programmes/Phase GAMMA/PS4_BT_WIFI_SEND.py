from pyPS4Controller.controller import Controller
import socket
import threading
import time

UDP_IP = "192.168.1.68" # "127.0.0.1" = loopback (pour ce l'envoyer à nous même), par ailleurs on ne peut pas broadcast
UDP_PORT = 5005
MESSAGE = ""

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

MAX = 32767
dead = 30
delay = 100*(10)**(-6)

# Structure des messages : 
# [valuePWM,L ou R,A ou R]
def Mapping(val, oldMax = MAX, newMax = 255) :
    return int(val * (newMax/oldMax))

class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)

    def on_x_press(self):
        MESSAGE = "B,1"
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)

    def on_triangle_press(self):
        print("on_triangle_press")

    def on_circle_press(self):
        print("on_circle_press")

    def on_square_press(self):
        print("on_square_press")

    def on_L1_press(self):
        print("on_L1_press")

    def on_L2_press(self, value):
        print("on_L2_press: {}".format(value))

    def on_R1_press(self):
        print("on_R1_press")

    def on_R2_press(self, value):
        print("on_R2_press: {}".format(value))

    def on_up_arrow_press(self):
        print("on_up_arrow_press")

    def on_down_arrow_press(self):
        print("on_down_arrow_press")

    def on_left_arrow_press(self):
        print("on_left_arrow_press")

    def on_right_arrow_press(self):
        print("on_right_arrow_press")

    def on_L3_up(self, value):
        value = abs(Mapping(value,MAX))
        if value >= dead :
            MESSAGE = str("M,") +str(value) + "," + str("L") + "," + str("A")
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)
            
    def on_L3_down(self, value):
        value = Mapping(value,MAX)
        if value >= dead :
            MESSAGE = str("M,") + str(value) + "," + str("L") + "," + str("R")
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)

    def on_L3_y_at_rest(self):
        MESSAGE = str("M,") +str(0) + "," + str("L")
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))

    def on_L3_press(self):
        """L3 joystick is clicked. This event is only detected when connecting without ds4drv"""
        print("on_L3_press")

    def on_R3_up(self, value):
        value = abs(Mapping(value,MAX))
        if value >= dead :
            MESSAGE = str("M,") +str(value) + "," + str("R") + "," + str("A")
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)

    def on_R3_down(self, value):
        value = Mapping(value,MAX)
        if value >= dead :
            MESSAGE = str("M,") +str(value) + "," + str("R") + "," + str("R")
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        time.sleep(delay)

    def on_R3_y_at_rest(self):
        MESSAGE = str("M,") +str(0) + "," + str("R")
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT)) 

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

    def on_L3_left(self, value):
        pass

    def on_L3_right(self, value):
        pass

    def on_L3_x_at_rest(self):
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

controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
controller.listen()
