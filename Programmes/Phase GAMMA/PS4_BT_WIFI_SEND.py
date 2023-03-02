from pyPS4Controller.controller import Controller
import socket
import threading


class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)

    def on_x_press(self):
        print("on_x_press")

    def on_x_release(self):
        pass

    def on_triangle_press(self):
        print("on_triangle_press")

    def on_triangle_release(self):
        pass

    def on_circle_press(self):
        print("on_circle_press")

    def on_circle_release(self):
        pass 

    def on_square_press(self):
        print("on_square_press")

    def on_square_release(self):
        pass

    def on_L1_press(self):
        print("on_L1_press")

    def on_L1_release(self):
        pass

    def on_L2_press(self, value):
        print("on_L2_press: {}".format(value))

    def on_L2_release(self):
        pass

    def on_R1_press(self):
        print("on_R1_press")

    def on_R1_release(self):
        pass

    def on_R2_press(self, value):
        print("on_R2_press: {}".format(value))

    def on_R2_release(self):
        pass

    def on_up_arrow_press(self):
        print("on_up_arrow_press")

    def on_up_down_arrow_release(self):
        pass

    def on_down_arrow_press(self):
        print("on_down_arrow_press")

    def on_left_arrow_press(self):
        print("on_left_arrow_press")

    def on_left_right_arrow_release(self):
        pass

    def on_right_arrow_press(self):
        print("on_right_arrow_press")

    def on_L3_up(self, value):
        print("on_L3_up: {}".format(value))

    def on_L3_down(self, value):
        print("on_L3_down: {}".format(value))

    def on_L3_left(self, value):
        pass

    def on_L3_right(self, value):
        pass

    def on_L3_y_at_rest(self):
        # mettre la valeur à 0
        pass

    def on_L3_x_at_rest(self):
        pass

    def on_L3_press(self):
        """L3 joystick is clicked. This event is only detected when connecting without ds4drv"""
        print("on_L3_press")

    def on_L3_release(self):
        pass

    def on_R3_up(self, value):
        print("on_R3_up: {}".format(value))

    def on_R3_down(self, value):
        print("on_R3_down: {}".format(value))

    def on_R3_left(self, value):
        pass

    def on_R3_right(self, value):
        pass

    def on_R3_y_at_rest(self):
        # mettre la valeur à 0 
        pass

    def on_R3_x_at_rest(self):
        pass

    def on_R3_press(self):
        """R3 joystick is clicked. This event is only detected when connecting without ds4drv"""
        print("on_R3_press")

    def on_R3_release(self):
        pass

    def on_options_press(self):
        print("on_options_press")

    def on_options_release(self):
        pass

    def on_share_press(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_share_press")

    def on_share_release(self):
        pass

    def on_playstation_button_press(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_playstation_button_press")

    def on_playstation_button_release(self):
        """this event is only detected when connecting without ds4drv"""
        print("on_playstation_button_release")

    


controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
controller.listen()