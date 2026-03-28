import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(5,  GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(6,  GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(19, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

try:
    while True:
        print ("-----")
        print ("TURBO: true" if GPIO.input(5) else "TURBO: false")
        print ("POWER_ON: true" if GPIO.input(6) else "POWER ON: false")
        print ("PAUSE: true" if GPIO.input(13) else "PAUSE: false")
        print ("TRIGGER: true" if GPIO.input(19) else "TRIGGER: false")
        print ("CURSOR: true" if GPIO.input (26) else "CURSOR: false")
        
        time.sleep(0.5)
finally:
    GPIO.cleanup()
