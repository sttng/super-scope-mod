#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2901.h>

#include <WiFi.h>               // This is the most basic WiFi implimentation
#include "driver/adc.h"         // Allows direct control of ADC for powersaving, can't be <adc.h> for some reason
#include <esp_bt.h>             // Allows direct API control of BT for powersaving
#include <esp_wifi.h>           // Provides direct API control of WiFi radio for powersaving

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define DEVICE_NAME         "SuperScopeMod_ESP32S3"  
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;

const int COMPOSITE_SYNC_PIN = 6;
const int VERTICAL_SYNC_PIN = 7;

const int TRIGGER_PIN = 5;
const int CURSOR_PIN = 1;
const int TURBO_PIN = 3;
const int PAUSE_PIN = 4;
const int EXTERNAL_LATCH_PIN = 2; // Raster gate

const byte BT_MESSAGE_TRIGGER_PRESSED = 231;
const byte BT_MESSAGE_TRIGGER_RELEASED = 232;
const byte BT_MESSAGE_CURSOR_PRESSED = 233;
const byte BT_MESSAGE_CURSOR_RELEASED = 234;
const byte BT_MESSAGE_TURBO_ENABLED = 235;
const byte BT_MESSAGE_TURBO_DISABLED = 236;
const byte BT_MESSAGE_PAUSE_PRESSED = 237;
const byte BT_MESSAGE_PAUSE_RELEASED = 238;
const byte BT_MESSAGE_TV_NOT_VISIBLE = 239;
const byte BT_MESSAGE_AIM_X = 240;
const byte BT_MESSAGE_AIM_Y = 241;

const byte BT_MESSAGE_NONE = 0;
byte lastBluetoothMessage = BT_MESSAGE_NONE;

bool isTriggerPressed = false;
bool isCursorPressed = false;
bool isTurboEnabled = false;
bool isPausePressed = false;
volatile bool isTvVisible = false;

const int minAimLine = 10;
const int maxAimLine = 240;
const int minAimMicroseconds = 5;
const int maxAimMicroseconds = 55;

volatile int aimLine = 120; //(maxAimLine - minAimLine + 1) / 2 + minAimLine;
volatile int aimMicroseconds = 27; //(maxAimMicroseconds - minAimMicroseconds + 1) / 2 + minAimMicroseconds;

volatile int currentLine = 0;
// Declare the flag as volatile since it's modified inside the ISR
volatile bool interruptCompositeSyncTriggered = false;
volatile bool interruptVerticalSyncTriggered = false;



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      //digitalWrite(48, HIGH);
      Serial.println("Client Connected");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
      //digitalWrite(48, LOW);
      Serial.println("Client Disconnected");
      deviceConnected = false;
      BLEDevice::startAdvertising();
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint32_t currentMillis = millis() / 1000;
    pCharacteristic->setValue(0);
  }
};

void setup() {

  esp_wifi_set_mode(WIFI_MODE_NULL);
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF);    // Switch WiFi off
  esp_wifi_stop();
  //adc_power_release(); 


  // Set the CPU frequency to 80 MHz for consumption optimization
  //setCpuFrequencyMhz(80);
  
  Serial.begin(9600);
  pinMode(48, OUTPUT);
  digitalWrite(48, HIGH);
  delay(1000);
  digitalWrite(48, LOW);
  delay(1000);
  
  // Print the XTAL crystal frequency
  //Serial.print("XTAL Crystal Frequency: ");
  //Serial.print(getXtalFrequencyMhz());
  //Serial.println(" MHz");

  // Print the CPU frequency
  Serial.print("CPU Frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  // Print the APB bus frequency
  Serial.print("APB Bus Frequency: ");
  Serial.print(getApbFrequency());
  Serial.println(" Hz");
  //End CPU frequency change
  
 

  // Create the BLE Device
  BLEDevice::init(DEVICE_NAME);

  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE);
  
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  //pCharacteristic->setValue("Super Scope Chars");

  // Descriptors
  BLE2901 *pDescriptor_2901 = new BLE2901();
  pDescriptor_2901->setDescription("SuperScope Data");
  pCharacteristic->addDescriptor(pDescriptor_2901);

  // Start the Service
  pService->start();

  // Start BLE advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  //pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  //Setup Super Scope Mod Pins, etc
  pinMode(COMPOSITE_SYNC_PIN, INPUT);
  pinMode(VERTICAL_SYNC_PIN, INPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(CURSOR_PIN, OUTPUT);
  pinMode(TURBO_PIN, OUTPUT);
  pinMode(PAUSE_PIN, OUTPUT);
  pinMode(EXTERNAL_LATCH_PIN, OUTPUT);

  digitalWrite(TRIGGER_PIN, HIGH);
  digitalWrite(CURSOR_PIN, HIGH);
  digitalWrite(TURBO_PIN, HIGH);
  digitalWrite(PAUSE_PIN, HIGH);
  digitalWrite(EXTERNAL_LATCH_PIN, HIGH);

  attachInterrupt(digitalPinToInterrupt(COMPOSITE_SYNC_PIN), handleCompositeSync, RISING);
  attachInterrupt(digitalPinToInterrupt(VERTICAL_SYNC_PIN), handleVerticalSync, RISING);
  // Finished Super Scope Mod Setup

  Serial.println("Super Scope Mod BLE receiver started.");

}

void loop() {

  if (interruptCompositeSyncTriggered) {
    //if (isTvVisible && currentLine == aimLine) {
    if (currentLine == aimLine) { 
      delayMicroseconds(aimMicroseconds);
      digitalWrite(EXTERNAL_LATCH_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(EXTERNAL_LATCH_PIN, HIGH);  
    }
    
    // Reset the flag and re-attach the interrupt
    interruptCompositeSyncTriggered = false;
    attachInterrupt(digitalPinToInterrupt(COMPOSITE_SYNC_PIN), handleCompositeSync, RISING);  
  }

  if (interruptVerticalSyncTriggered) {
    interruptVerticalSyncTriggered = false;
    attachInterrupt(digitalPinToInterrupt(VERTICAL_SYNC_PIN), handleVerticalSync, RISING);
  }

  // Process commands when a device is connected.
  if (deviceConnected && pCharacteristic->getValue().length() > 0) {
    String value = pCharacteristic->getValue().c_str();

    //Serial.println(value.toInt());
    int inputByte = value.toInt();

    switch (inputByte) {
      case BT_MESSAGE_TRIGGER_PRESSED:
        digitalWrite(TRIGGER_PIN, LOW);

        delay(250);
        digitalWrite(TRIGGER_PIN, HIGH);

        Serial.println("Trigger pressed.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_TRIGGER_RELEASED:
        digitalWrite(TRIGGER_PIN, HIGH);
        Serial.println("Trigger released.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_CURSOR_PRESSED:
        digitalWrite(CURSOR_PIN, LOW);
        Serial.println("Cursor pressed.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_CURSOR_RELEASED:
        digitalWrite(CURSOR_PIN, HIGH);
        Serial.println("Cursor released.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_TURBO_ENABLED:
        digitalWrite(TURBO_PIN, LOW);
        Serial.println("Turbo enabled.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_TURBO_DISABLED:
        digitalWrite(TURBO_PIN, HIGH);
        Serial.println("Turbo disabled.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_PAUSE_PRESSED:
        digitalWrite(PAUSE_PIN, LOW);
        Serial.println("Pause pressed.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_PAUSE_RELEASED:
        digitalWrite(PAUSE_PIN, HIGH);
        Serial.println("Pause released.");
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_TV_NOT_VISIBLE:
        isTvVisible = false;
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
      case BT_MESSAGE_AIM_X:
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
      case BT_MESSAGE_AIM_Y:
        isTvVisible = true;
        pCharacteristic->setValue(0); //Set Characterstic back to 0.
        break;
    }

    /*if (inputByte > 230) {
      lastBluetoothMessage = inputByte;
      pCharacteristic->setValue(0); //Set Characterstic back to 0.
    }
    else {
      switch (lastBluetoothMessage) {
        case BT_MESSAGE_AIM_X:
          aimMicroseconds = inputByte + minAimMicroseconds;
          pCharacteristic->setValue(0); //Set Characterstic back to 0.
          break;
        case BT_MESSAGE_AIM_Y:
          aimLine = inputByte + minAimLine;
          pCharacteristic->setValue(0); //Set Characterstic back to 0.
          break;
      }
    }*/
  }
}

void ARDUINO_ISR_ATTR handleCompositeSync() {
  currentLine++;
  interruptCompositeSyncTriggered = true; // Set a flag. DO NOT do heavy processing here!
  
  // Disable interrupt temporarily if you need to perform actions safely
  detachInterrupt(digitalPinToInterrupt(COMPOSITE_SYNC_PIN));
}

void ARDUINO_ISR_ATTR handleVerticalSync() {
  currentLine = 0;
  interruptVerticalSyncTriggered = true; // Set a flag. DO NOT do heavy processing here!

  // Disable interrupt temporarily if you need to perform actions safely
  detachInterrupt(digitalPinToInterrupt(VERTICAL_SYNC_PIN));
}