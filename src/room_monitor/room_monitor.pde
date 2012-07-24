#include "Clock.h"

const int DATA_BUFFER_LENGTH = 150;
class Data{
public:
  void recordSensors(long time, double temperature, int light);
  void printRecordedToSerial();
  void resetRecordings();
private:

  int dataPos;
  long times[DATA_BUFFER_LENGTH];
  int temperatures[DATA_BUFFER_LENGTH];
  int lights[DATA_BUFFER_LENGTH];
};


Clock clock(0);
Data data;

void setup(){
  initSerial();
  data.resetRecordings();

  analogReference(INTERNAL); // 3.3V

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  blink(13, 1000, 200);
  blink(12, 300, 200);
  blink(11, 300, 200);
}

void monitorSerial(void (*f)(char*));

void loop(){      
  data.recordSensors(clock.time(), readTemperature(), readLight());

  Serial.print(clock.time(), DEC);
  Serial.print("] Temp: ");
  double temperature = readTemperature();
  //  Serial.print(analogRead(0));
  //  Serial.print(",");
  Serial.print(temperature, 1);
  Serial.print(", Light: ");
  Serial.print(readLight());
  Serial.println();

  for (int i= 0; i< 20; i++){
    monitorSerial(parseCommand);
    clock.updateTime();
    delay(500);
  }
}


void yield(){
  clock.updateTime();
}


void parseCommand(char* line){
  String l = String(line);
  if (l.substring(0, 4) == "ECHO"){
    Serial.print("Echo: ");
    Serial.println(l.substring(3));
    return;
  }
  if (l.substring(0, 5) == "FETCH"){
    data.printRecordedToSerial();
    data.resetRecordings();
    return;
  }

  Serial.println();
  Serial.print("Unknown Command:");
  Serial.println(l); 
}

// Sensors -----------------------------------------------------------

int readLight(){
  return analogRead(1);
}

double readTemperature(){
  int READS = 1;
  int temp= 0;
  for (int i =0; i< READS; i++){
    temp += analogRead(0);
  }
  temp = temp / READS;

  return ((double(temp)*110.0)/ 1023.0  );
}


// Diodes ---------------------------------------------------

void blink(int pin, int on, int off){
  digitalWrite(pin, HIGH);
  delay(on);
  digitalWrite(pin, LOW);
  delay(off);
}


// DATA BUFFER -----------------------------------------------------------



void Data::recordSensors(long time, double temperature, int light){

  digitalWrite(12, LOW);  
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);

  if (dataPos >= DATA_BUFFER_LENGTH/2 && dataPos < DATA_BUFFER_LENGTH) {
    digitalWrite(12, HIGH);
  } 


  if(dataPos >= DATA_BUFFER_LENGTH) {
    Serial.println("DATA BUFFER OVERFLOW");
    digitalWrite(11, HIGH);
    return;
  }

  if ( dataPos > 0 &&
    (abs(temperatures[dataPos-1] - int(10*temperature)) < 5 ) &&
    (abs(lights[dataPos-1] - light) < 50)
    ) {
    return;
  }

  //  Serial.println(abs(temperatures[dataPos] - int(10*temperature)));
  //  Serial.println(abs(lights[dataPos] - light));

  digitalWrite(13,HIGH);

  times[dataPos] = time;
  temperatures[dataPos] = int(10*temperature);
  lights[dataPos] = light;
  dataPos++;
}

void Data::printRecordedToSerial(){
  for(int i =0; i< dataPos; i++){
    Serial.print(times[i], DEC);
    Serial.print(";");
    Serial.print(double(temperatures[i])/10.0, 1);
    Serial.print(";");
    Serial.print(lights[i], DEC);
    Serial.println();
  }
}

void Data::resetRecordings(){
  dataPos = 0;
}

// -----------------------------------------------------------



// Serial ---------------------------

const int BUFFER_SIZE = 100;
char buffer[BUFFER_SIZE + 1];
int bufferPos;

void initSerial(){
  Serial.begin(9600);
  bufferPos = 0;
}

void monitorSerial(void (*onLineRead)(char*)){
  while (Serial.available()) {
    char lastReadChar = char(Serial.read());
    buffer[bufferPos] = lastReadChar;
    bufferPos = bufferPos + 1;
    if (lastReadChar == '\n' || lastReadChar == '\r' || bufferPos == BUFFER_SIZE){
      buffer[bufferPos] = 0;
      yield();
      (*onLineRead)(buffer);
      bufferPos = 0;
    }
  }
}


