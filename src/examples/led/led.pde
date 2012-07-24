const int LED = 12;
const int R = 2;
const int Y = 3;
const int G = 4;

const int G2 = 6;
const int R2 = 5;

const int SW = 7;
const int SOUND = 8;

unsigned lastCrossing;
boolean crossingRequested;

void setup(){
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(SOUND, OUTPUT);
  
  pinMode(SW, INPUT);
  crossingRequested = false;
  lastCrossing = millis() - 1000000;
  lightChange(LOW, LOW, HIGH, HIGH, LOW, 5);
}

void loop(){  
  
  if (digitalRead(SW) == HIGH) {
    crossingRequested = true;
    ledOn(13, 300);
  }
  
  if (crossingRequested && ((millis() - lastCrossing) > 5000)){ 
    lastCrossing = millis();
    crossingRequested = false;
    doCrossing();
    
  }
}

void doCrossing(){
    lightChange(LOW, HIGH, HIGH, HIGH, LOW, 1000);
    lightChange(HIGH, LOW, LOW, LOW, HIGH, 0);
    for(int i=0; i< 10; i++){
      beep(SOUND, 300, 150);    
      delay(150);
    }
    blinking(10,100);
    lightChange(LOW, LOW, HIGH, HIGH, LOW, 2500);
}

void blinking(int times, int interval){
  for(int i = 0; i<times; i++){
     lightChange(LOW, LOW, LOW, LOW, LOW, interval);
     lightChange(LOW, HIGH, LOW, LOW, HIGH, interval);
  }
}

void ledOn(int pin, int time){
  digitalWrite(pin, HIGH);
  digitalWrite(13, HIGH);
  delay(time);
  digitalWrite(pin, LOW);
  digitalWrite(13, LOW);
}

void lightChange(boolean r, boolean y, boolean g, boolean r2, boolean g2, int _delay){
  digitalWrite(R, r);
  digitalWrite(Y, y);
  digitalWrite(G, g);  
  digitalWrite(R2, r2);  
  digitalWrite(G2, g2);  
  delay(_delay);
}


void beep(int sounderPin, int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(sounderPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(sounderPin, LOW);
    delayMicroseconds(tone);
  }
}



