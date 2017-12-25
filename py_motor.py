#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time
import signal
import sys
import numpy as np

def exit_handler(signal, frame):
    print("\nExit")
    servo.stop()
    GPIO.cleanup()
    sys.exit(0)

upperbound = 10.75
lowerbound = 2.5
width = upperbound - lowerbound
center = (upperbound + lowerbound) / 2

def move(pin_number, cycle):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin_number, GPIO.OUT)
    servo = GPIO.PWM(pin_number, 50)
    servo.start(cycle)
    servo.ChangeDutyCycle(cycle)
    time.sleep(1.5)
    GPIO.cleanup()

def gradualMove(pin_number, target_cycle, cur_cycle):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin_number, GPIO.OUT)
    servo = GPIO.PWM(pin_number, 50)
    servo.start(cur_cycle)
    step = 0.05
    if target_cycle > cur_cycle:
        array = np.arange(cur_cycle, target_cycle, step)
    else:
        array = np.arange(cur_cycle - 1.0, target_cycle, -step)
    for i in array:
        #print(i)
        servo.ChangeDutyCycle(i)
        time.sleep(step)
    GPIO.cleanup()

if __name__ == "__main__":
    signal.signal(signal.SIGINT, exit_handler)
    move(32, 7.25)
    gradualMove(12, 9.2, 6.0)
    print("down")
    time.sleep(2.5)
    gradualMove(12, 6.0, 9.2)
    print("up")
    time.sleep(0.5)
