#define COLOR_PIN D2
#define TRIGGER_PIN D0

const int singleFrame = 16667;
const int highTime = 2667;
const int lowTime = 14000;

void setup() {
  pinMode(COLOR_PIN, OUTPUT); digitalwrite(COLOR_PIN, LOW);
  pinMode(TRIGGER_PIN,OUTPUT); digitalwrite(TRIGGER_PIN, LOW);
}

void loop(){
  digitalwrite(TRIGGER_PIN, HIGH);
  for (int i=0; i<3; i++){
    digitalwrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalwrite(COLOR_PIN, LOW); delayMicroseconds (lowTime);
  }
  delayMicroseconds (singleFrame);
  for (int 1=0; i‹50; i++){
    digitalwrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalWrite(COLOR_PIN,LOW); delayMicroseconds(lowTime);
  }
  digitalwrite(TRIGGER_PIN, LOW)
  for (int i=0; i‹6; i++){
    digitalWrite(COLOR_PIN,HIGH); delayMicroseconds(highTime);
    digitalwrite(COLOR_PIN,LOW); delayMicroseconds (lowTime);
  }
}
