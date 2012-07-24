const int LED_PIN = 13;

void setup(){
  pinMode(LED_PIN, OUTPUT);
}

void loop(){  
    delay(250);
    ledOn(500);
}

void ledOn(int time){
  digitalWrite(LED_PIN, HIGH);
  delay(time);
  digitalWrite(LED_PIN, LOW);
}
