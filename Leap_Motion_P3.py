import os
import sys
import io
import time
import Leap
import numpy as np
import threading
from threading import Thread
from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture

# Global flag used to keep track of the leap motion connection
Leap_connected = False


class LeapThread(Thread):
    '''
    Provides a dedicated thread for Leap Motion declarations and stuff.
    '''

    def __init__(self, name):
        Thread.__init__(self)
        self.name = name

    def run(self):
        try:
            # Create a sample listener and controller
            listener = SampleListener()
            controller = Leap.Controller()

            # Have the sample listener receive events from the controller
            controller.add_listener(listener)

            print("Waiting for Leap Motion connection...\n")

            # This loop ensure that this thread remains active until the user decides to exit the app
            while(True):
                time.sleep(0.5)

        # If ANY exception is raised then the specified thread will be closed
        except:
            print("\n\nThe Leap Motion thread raised an exception. Terminating...\n\n")


class SampleListener(Leap.Listener):
    finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']

    def on_init(self, controller):
        print("Initialized\n")

    def on_connect(self, controller):
        global Leap_connected

        print("Connected\n")
        Leap_connected = True

        # Enable gestures
        controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE)
        controller.enable_gesture(Leap.Gesture.TYPE_KEY_TAP)
        controller.enable_gesture(Leap.Gesture.TYPE_SCREEN_TAP)
        controller.enable_gesture(Leap.Gesture.TYPE_SWIPE)

    def on_disconnect(self, controller):
        print("Disconnected\n")

    def on_exit(self, controller):
        print("Exited Leap Motion\n")

    def on_frame(self, controller):

        # Get the most recent frame
        frame = controller.frame()

        # Get hands
        hands = frame.hands
        arms = frame.arm
        fingers = frame.finger
        tools = frame.tools

        # Get gesture as list
        gestures = list(frame.gestures())

        # Get the number of each feature so that we can decide which one to work on
        numHands = len(hands)
        numArms = len(arms)
        numFingers = len(fingers)
        numTools = len(tools)
        numGestures = len(gestures)

        if numHands >= 1:
            # Get the first hand or we can use a "for hand in hands" and use the same index to specify the arm
            hand = hands[0]
            arm = arms[0]

            # Get the palm position
            normal = hand.palm_normal
            direction = hand.direction

            print("Hand: position %s, pitch: %f degrees, roll: %f degrees, yaw: %f degrees" % (
                hand.palm_position, direction.pitch * Leap.RAD_TO_DEG, normal.roll * Leap.RAD_TO_DEG, direction.yaw * Leap.RAD_TO_DEG))

            print("Arm direction: %s, wrist position: %s, elbow position: %s" %
                  (arm.direction, arm.wrist_position, arm.elbow_position))

        # Tools still a wip
        if numTools >= 1:
            tool = tools[0]
            print("Tool position: %s, direction: %s" %
                  (tool.tip_position, tool.direction))

        if numGestures >= 1:
            for gesture in gestures:

                if gesture.type == Leap.Gesture.TYPE_SWIPE:
                    swipe = SwipeGesture(gesture)
                    print("Swipe state: %s, position: %s, direction: %s, speed: %f" % (
                        self.state_names[gesture.state], swipe.position, swipe.direction, swipe.speed))

                if gesture.type == Leap.Gesture.TYPE_KEY_TAP:
                    keytap = KeyTapGesture(gesture)
                    print("Key Tap: %s, position: %s, direction: %s" % (
                        gesture.id, self.state_names[gesture.state], keytap.position, keytap.direction))

                if gesture.type == Leap.Gesture.TYPE_SCREEN_TAP:
                    screentap = ScreenTapGesture(gesture)
                    print("  Screen Tap: %s, position: %s, direction: %s" % (
                        self.state_names[gesture.state], screentap.position, screentap.direction))

    # Don't even know why I've made this tbh....since I got the state_names[] at the top.
    def state_string(self, state):
        if state == Leap.Gesture.STATE_START:
            return "STATE_START"

        if state == Leap.Gesture.STATE_UPDATE:
            return "STATE_UPDATE"

        if state == Leap.Gesture.STATE_STOP:
            return "STATE_STOP"

        if state == Leap.Gesture.STATE_INVALID:
            return "STATE_INVALID"


def LeapMask_main():
    global Leap_connected
    try:
        print("\n######## Leap Motion ########\n")

        # Create a Leap Motion thread that will handle connection and data gathering
        Leap_thread = LeapThread("Leap")
        Leap_thread.daemon = True
        Leap_thread.start()

        # Initialize a time-out timer variable
        time_out_timer = 0

        # Wait (10 sec) for the Leap Motion connected event to set the global flag.
        # If passed that timeframe the flag is still set to false exit the whole application
        while(Leap_connected == False):
            time.sleep(0.5)
            time_out_timer += 0.5
            if(time_out_timer >= 10):
                print(
                    "Timeout: cannot connect to the Leap Motion sensor in time :(\nExiting...\n")
                sys.exit()

        # Loop to keep the thread of main alive
        while(True):
            time.sleep(0.1)

    except:
        sys.exit()


if __name__ == '__main__':
    # Call main function and pass args given by user
    LeapMask_main()
