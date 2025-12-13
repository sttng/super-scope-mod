const int COMPOSITE_SYNC PIN = 2;
const int VERTICAL SYNC_PIN = 3;

volatile int lineCount = 0;
volatile int previousLineCount = 0;

volatile unsigned long linestartTime = 0;
volatile unsigned long previouslineStartTime = 0;
volatile unsigned long elapsedlineTime = 0;

volatile int frameCounter = 0;
volatile bool inVsync = false;

void setup () {
  Serial.begin(9600);

  pinMode (COMPOSITE_SYNC_PIN, INPUT);
  pinMode (VERTICAL_ SYNC_PIN, INPUT);

  attachInterrupt (digitalPinToInterrupt (COMPOSITE_SYNC_PIN), handleCompositesync, RISING;
  attachInterrupt (digitalPinToInterrupt (VERTICAL_SYNC_PIN), handleVerticalsync, CHANGE);
}
