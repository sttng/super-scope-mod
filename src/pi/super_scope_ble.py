import cv2
import numpy
import os
import RPi.GPIO as GPIO
import time

from imutils import perspective

import simplepyble

BLE_REMOTE_ADDRESS = "74:4D:BD:65:D3:E9" #"D0:CF:13:2F:C3:C5"
SERVICE_UUID ="4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
DEFAULT_PI_BLE_ADAPTER_IDX = 0

TRACKING_WIDTH = 51
BLACK_BAR_WIDTH = 8
WIDE_TRACKING_WIDTH = TRACKING_WIDTH + BLACK_BAR_WIDTH * 2
TRACKING_HEIGHT = 231

TURBO_PIN = 5
POWER_ON_PIN = 6
PAUSE_BUTTON_PIN = 13
TRIGGER_BUTTON_PIN = 19
CURSOR_BUTTON_PIN = 26

BT_MESSAGE_TRIGGER_PRESSED = "231"
BT_MESSAGE_TRIGGER_RELEASED = "232"
BT_MESSAGE_CURSOR_PRESSED = "233"
BT_MESSAGE_CURSOR_RELEASED = "234"
BT_MESSAGE_TURBO_ENABLED = "235"
BT_MESSAGE_TURBO_DISABLED = "236"
BT_MESSAGE_PAUSE_PRESSED = "237"
BT_MESSAGE_PAUSE_RELEASED = "238"
BT_MESSAGE_TV_NOT_VISIBLE = "239"
BT_MESSAGE_AIM_X = "240"
BT_MESSAGE_AIM_Y = "241"

isTvVisible = False

GPIO.setmode(GPIO.BCM)

GPIO.setup(TURBO_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(POWER_ON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(PAUSE_BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(TRIGGER_BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(CURSOR_BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def on_scan_start():
    print("Scan started.")
def on_scan_stopped():
    print("Scan complete.")
def on_device_found(peripheral):
    if(peripheral.address() == BLE_REMOTE_ADDRESS):
        print(f"Found {peripheral.identifier()} [{peripheral.address()}]")

adapters = simplepyble.Adapter.get_adapters()
if len(adapters) == 0:
    print("No adapters found")
adapter = adapters[DEFAULT_PI_BLE_ADAPTER_IDX]
print(f"Selected Pi adapter: {adapter.identifier()} [{adapter.address()}]")
adapter.set_callback_on_scan_start(on_scan_start)
adapter.set_callback_on_scan_stop(on_scan_stopped)
adapter.set_callback_on_scan_found(on_device_found)
adapter.scan_for(5000)
    
peripherals = adapter.scan_get_results()
selectedperi = 0

for i, peripheral in enumerate(peripherals):
    if(peripheral.address() == BLE_REMOTE_ADDRESS):
        selectedperi = peripheral
            
peripheral = selectedperi
print(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
peripheral.connect()
service_uuid = SERVICE_UUID
characteristic_uuid = CHARACTERISTIC_UUID


def handleButtons(channel):
    isRising = GPIO.input(channel)

    if channel == TURBO_PIN:
        peripheral.write_request(service_uuid, characteristic_uuid, str.encode(BT_MESSAGE_TURBO_ENABLED) if isRising else str.encode(BT_MESSAGE_TURBO_DISABLED))
    elif channel == POWER_ON_PIN:
        if not isRising:
            os.system("shutdown now -h")
    elif channel == PAUSE_BUTTON_PIN:
        peripheral.write_request(service_uuid, characteristic_uuid, str.encode(BT_MESSAGE_PAUSE_PRESSED) if isRising else str.encode(BT_MESSAGE_PAUSE_RELEASED))
    elif channel == TRIGGER_BUTTON_PIN:
        peripheral.write_request(service_uuid, characteristic_uuid, str.encode(BT_MESSAGE_TRIGGER_PRESSED) if isRising else str.encode(BT_MESSAGE_TRIGGER_RELEASED))
        print("Trigger")
    elif channel == CURSOR_BUTTON_PIN:
        peripheral.write_request(service_uuid, characteristic_uuid, str.encode(BT_MESSAGE_CURSOR_PRESSED) if isRising else str.encode(BT_MESSAGE_CURSOR_RELEASED))

handleButtons(TURBO_PIN)

GPIO.add_event_detect(TURBO_PIN, GPIO.BOTH, callback = handleButtons, bouncetime = 10)
GPIO.add_event_detect(POWER_ON_PIN, GPIO.BOTH, callback = handleButtons, bouncetime = 10)
GPIO.add_event_detect(PAUSE_BUTTON_PIN, GPIO.BOTH, callback = handleButtons, bouncetime = 10)
GPIO.add_event_detect(TRIGGER_BUTTON_PIN, GPIO.BOTH, callback = handleButtons, bouncetime = 10)
GPIO.add_event_detect(CURSOR_BUTTON_PIN, GPIO.BOTH, callback = handleButtons, bouncetime = 10)


while True:
    print ("-----")
    time.sleep(0.5)
