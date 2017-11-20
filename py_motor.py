#/usr/bin/env python3
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time
import signal
import sys

def exit_handler(signal, frame):
    print("\nExit")
    servo.stop()
    GPIO.cleanup()
    sys.exit(0)

upperbound = 11
lowerbound = 2.5
width = upperbound - lowerbound
center = (upperbound + lowerbound) / 2

def move(pin_number, angle):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin_number, GPIO.OUT)
    servo = GPIO.PWM(pin_number, 50)
    if -90 <= angle and angle <= 90:
        #角度から変換
        cycle = angle * width / 2 / 90 + center
        servo.ChangeDutyCycle(cycle)
        time.sleep(0.5)
    GPIO.cleanup()

def move2(pin_number, cycle):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin_number, GPIO.OUT)
    servo = GPIO.PWM(pin_number, 50)
    servo.start(cycle)
    servo.ChangeDutyCycle(cycle)
    time.sleep(0.5)
    GPIO.cleanup()

if __name__ == "__main__":
    signal.signal(signal.SIGINT, exit_handler)
    move2(35, 8)
    time.sleep(0.5)
    move2(35, 6)
    time.sleep(0.5)
    GPIO.cleanup()
