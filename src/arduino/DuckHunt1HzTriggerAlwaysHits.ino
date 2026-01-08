#define COLOR_PIN 4
#define TRIGGER_PIN 6

const int singleFrame = 16667;
const int highTime = 2667;
const int lowTime = 14000;

void setup() {
  pinMode(COLOR_PIN, OUTPUT); digitalWrite(COLOR_PIN, LOW);
  pinMode(TRIGGER_PIN,OUTPUT); digitalWrite(TRIGGER_PIN, LOW);
}

void loop(){
  digitalWrite(TRIGGER_PIN, HIGH);
  for (int i = 0; i < 3; i++){
    digitalWrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalWrite(COLOR_PIN, LOW); delayMicroseconds (lowTime);
  }
  delayMicroseconds (singleFrame);
  for (int i = 0; i < 50; i++){
    digitalWrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalWrite(COLOR_PIN,LOW); delayMicroseconds(lowTime);
  }
  digitalWrite(TRIGGER_PIN, LOW);
  for (int i = 0; i < 6; i++){
    digitalWrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalWrite(COLOR_PIN,LOW); delayMicroseconds (lowTime);
  }
}
