const int pulsePinA = 2;  
const int pulsePinB = 3;  
int count = 0;  
int pulse_per_rev = 470;
int interval = 1000; //interval at which we will reset the count of pulses
long prevmillis = 0;
long currentmillis = 0;
//int pulseA,pulseB;

void setup() {
  
  pinMode(pulsePinA, INPUT_PULLUP);
  pinMode(pulsePinB, INPUT_PULLUP);
  
  
  attachInterrupt(digitalPinToInterrupt(pulsePinA), counter, FALLING);

 
  Serial.begin(9600);
  prevmillis = millis();
}

void loop() {
  //Serial.println(count);
  //Serial.println(pulseA);
  //Serial.println(pulseB);
 // int pulseA = digitalRead(pulsePinA);
 // int pulseB = digitalRead(pulsePinB);

 // delay(200); 
  currentmillis = millis();
  if (currentmillis - prevmillis > interval) {
    float rpm = (count/ pulse_per_rev);
    Serial.print("rpm : ");
    Serial.println(rpm);
    count = 0;
    prevmillis = currentmillis;
  }
}


void counter() {
  //int pulseA = digitalRead(pulsePinA);
  //int pulseB = digitalRead(pulsePinB);


  if(digitalRead(pulsePinB) == HIGH){
    count++;
  }
  else{
    count--;
  }  
}
