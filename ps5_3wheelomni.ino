//Author : shreyaskumbhar185,ShreyashCode001

#include <ps5Controller.h>
#include <esp_system.h>

//????????????????????????????????? BOT 1 GREEN ???????????????????????????????????

//DEFINING VARIABLES

int pwmLF=23, pwmRF=21, pwmB=18;
int dirLF=22, dirRF=19, dirB=17;
float Lx,Rx,Ly,Ry,z;
// bool R1;

//SETUP
void setup() {
  Serial.begin(115200);

  ps5.begin("88:03:4C:93:FE:13"); //replace with MAC address of your controller

 //TRY CONNECTING TO PS5 CONTROLLER

  while (ps5.isConnected()==0) {
    Serial.println("Connecting...");
    }
  Serial.println("Ready.");
  
  pinMode(pwmLF,OUTPUT);
  pinMode(pwmRF,OUTPUT);
  pinMode(pwmB,OUTPUT);
  pinMode(dirLF,OUTPUT);
  pinMode(dirRF,OUTPUT);
  pinMode(dirB,OUTPUT);

}
//CONTROLS
void Controls()
{
  Lx=ps5.LStickX();
  Ly=ps5.LStickY(); 
  Ry=ps5.RStickY();
  Rx=ps5.RStickX();
  // R1=ps5.R1();
}
//FUNCTIONS FOR dirECTION
float mapp(float control){
  float z;
  if((-128<=control)&&(control<=-30))
  {
    z=map(control,-30,-128,0,150);//mapping negative values
  }
  else if((30<=control)&&(control<=128))//mapping positive values
  {
    z=map(control,30,128,0,150);
  }
  else
  z =0;
  return z;
}

//FUNCTIONS FOR MOVEMENT
void Left(float control)   
{
  z = mapp(control);
  digitalWrite(dirB,LOW);
  analogWrite(pwmB,z);
  digitalWrite(dirRF,HIGH);
  analogWrite(pwmRF,z/1.80);
  digitalWrite(dirLF,HIGH);
  analogWrite(pwmLF,z/1.80);
}


void Right(float control)  
{
  z = mapp(control);
  digitalWrite(dirB,HIGH);
  analogWrite(pwmB,z);
  digitalWrite(dirLF,LOW);
  analogWrite(pwmLF,z/1.80);
  digitalWrite(dirRF,LOW);
  analogWrite(pwmRF,z/1.80); 
}

void clockwise(float control)
{
  z = mapp(control);
  digitalWrite(dirLF,HIGH);
  analogWrite(pwmLF,z);
  digitalWrite(dirRF,HIGH);
  analogWrite(pwmRF,z);
  digitalWrite(dirB,HIGH);
  analogWrite(pwmB,z);
}

void anticlockwise(float control)
{
  z = mapp(control);
  digitalWrite(dirB,LOW);
  analogWrite(pwmB,z);
  digitalWrite(dirLF,LOW);
  analogWrite(pwmLF,z);
  digitalWrite(dirRF,LOW);
  analogWrite(pwmRF,z);
}


void stop()
{
        Serial.println("i am in stop");
  if(z>0)
  {
      z -=10;
  analogWrite(pwmLF,z);
  analogWrite(pwmRF,z);
  analogWrite(pwmB,z);
  
      delay(100);
  }

  analogWrite(pwmLF,0);
  analogWrite(pwmRF,0);
  analogWrite(pwmB,0);
  
} 

void Front(float control) 
{
  z = mapp(control);
  digitalWrite(dirLF,HIGH);
  analogWrite(pwmLF,z);
  digitalWrite(dirRF,LOW);
  analogWrite(pwmRF,z);
  analogWrite(pwmB,0);
  
 
}

void Back(float control) 
{
  z = mapp(control);
  digitalWrite(dirLF,LOW);
  analogWrite(pwmLF,z);
  digitalWrite(dirRF,HIGH);
  analogWrite(pwmRF,z);
  analogWrite(pwmB,0);

}


void loop() {
  
  Controls();
    
 if (Ly > 30) {
    Front(Ly);
    Serial.println(Ly);
  } 
  else if (Ly < -30) {
        Serial.println(Ly);
    Back(Ly);
  } 
  else if (Lx < -30) {
        Serial.println(Lx);
    Left(Lx);
  } 
  else if (Lx > 30) {
        Serial.println(Lx);
    Right(Lx);
  } 
  else if (Rx > 30) {
        Serial.println(Rx);
    clockwise(Rx);
  } 
  else if (Rx < -30) {
        Serial.println(Rx);
    anticlockwise(Rx);
  } 
  else {
    stop();
  }
}
