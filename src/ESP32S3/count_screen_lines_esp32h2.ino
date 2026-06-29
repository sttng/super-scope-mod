/*
  For ESP32-H2 Dev Module

For serial output to work:
  Select Tools > USB CDC On Boot > Enabled from the Arduino IDE menus.
*/

const int COMPOSITE_SYNC_PIN = 5;
const int VERTICAL_SYNC_PIN = 8;

volatile int lineCount = 0;
volatile int previousLineCount = 0;

volatile unsigned long lineStartTime = 0;
volatile unsigned long previousLineStartTime = 0;
volatile unsigned long elapsedLineTime = 0;

volatile int frameCounter = 0;
volatile bool inVsync = false;

void setup() {
  Serial.begin(9600);

  pinMode(COMPOSITE_SYNC_PIN, INPUT);
  pinMode(VERTICAL_SYNC_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(COMPOSITE_SYNC_PIN), handleCompositeSync, RISING);
  attachInterrupt(digitalPinToInterrupt(VERTICAL_SYNC_PIN), handleVerticalSync, CHANGE);
}

void loop() {
  //Serial.println(previousLineCount);
  if (lineCount == 0 && previousLineCount > 15) {
    Serial.println(previousLineCount);
  }
}

void handleCompositeSync() {
  lineCount++;
}

void handleVerticalSync() {
  previousLineCount = lineCount;
  lineCount = 0;
}
