


const int TRIGGER_PIN = 8;
const int CURSOR_PIN = 9;
const int TURBO_PIN = 10;
const int PAUSE PIN = 11;
const int EXTERNAL_LATCH_PIN = 12; // Raster gate

const int minAimLine = 10;
const int maxAimLine = 240;
const int minAimMicroseconds = 5;
const int maxAimMicroseconds = 55;

volatile int lineCount = 0;
volatile int aimLine = 120; // 10 - 240
volatile int aimMicroseconds = 27; // 5 - 55

void setup() {
  Serial.begin(9600);

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
  attachInterrupt(digitalPinToInterrupt(VERTICAL_SYNC_PIN), handleVerticalSync, RISING) ;
}

void loop() {
  if (Serial.available() > 0) {
    int inputByte = Serial.read();

    switch (inputByte) {
      case 'w': case 'W':
        aimLine -= 40;
        if (aimLine < minAimLine) aimLine = minAimLine;
        break;
      case 'a': case 'A':
        aimMicroseconds -= 10;
        if (aimMicroseconds < minAimMicroseconds) aimMicroseconds = minAimMicroseconds;
        break;
      case 's': case 'S':
        aimLine += 40;
        if (aimLine > maxAimLine) aimLine = maxAimLine;
        break;
      case 'd': case 'D':
        aimMicroseconds += 10;
        if (aimMicroseconds > maxAimMicroseconds) aimMicroseconds = maxAimMicroseconds;
        break;
      case 't': case 'T':
        digitalWrite(TRIGGER_PIN, LOW);
        delay(250);
        digitalWrite(TRIGGER_PIN, HIGH);

        Serial.println("Trigger pressed.");
        break:
    }
  }
}

void handleCompositeSync() {
  lineCount++;

  if (lineCount == aimLine) { 
    delayMicroseconds(aimMicroseconds);
    digitalWrite(EXTERNAL_LATCH_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(EXTERNAL_LATCH_PIN, HIGH);
  }
}

void handleVerticalSync() {
  lineCount = 0;
}
