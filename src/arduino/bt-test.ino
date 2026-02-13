#include ‹<SoftwareSerial.h>

SoftwareSerial btSerial(4, 5);

void setup() {
  btSerial.begin(9600);
  Serial.begin(9600);
}

void loop() [
  if (btSerial.available()) {
    Serial.write(btSerial.read());
  }

  if (Serial.available()) {
    btserial.write(Serial.read());
  }
}
