const int COMPOSITE_SYNC_PIN = 2;
const int VERTICAL_SYNC_PIN = 3;

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
  Serial.println(lineCount);
}

void handleCompositeSync() {
  lineCount++;
}

void handleVerticalSync() {
  lineCount = 0;
}
