#include <ps5Controller.h>



//DEFINING VARIABLES
int pwmLF=17,pwmRF=22,pwmLB=19,pwmRB=16,DirLF=18,DirRF=23,DirLB=21 ,DirRB=4;
float Lx,Rx,Ly,Ry,z;

//SETUP
void setup() {
  Serial.begin(115200);
  //Ps3.begin("00:00:00:00:00:0a");
  
  //Serial.println("Bluetooth Started. Waiting for connection...");
  ps5.begin("88:03:4C:93:FE:13"); //replace with MAC address of your controller

 //TRY CONNECTING TO PS5 CONTROLLER

  while (ps5.isConnected()==0) {
    Serial.println("Connecting...");
    }
    
  //SerialBT.println("Ready.");
  Serial.println("Ready.");
  
  pinMode(pwmLF,OUTPUT);
  pinMode(pwmRF,OUTPUT);
  pinMode(pwmLB,OUTPUT);
  pinMode(pwmRB,OUTPUT);
  pinMode(DirLF,OUTPUT);
  pinMode(DirRF,OUTPUT);
  pinMode(DirLB,OUTPUT);
  pinMode(DirRB,OUTPUT);

}
//CONTROLS
void Controls()
{
  Lx=ps5.LStickX();
  Ly=ps5.LStickY(); 
  Ry=ps5.RStickY();
  Rx=ps5.RStickX();
}
//FUNCTIONS FOR DIRECTION
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
void front(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,HIGH);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,HIGH);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
 
}


void back(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,LOW);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,LOW);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
 
}

void clockwise(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,HIGH);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,LOW);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
 
}

void anticlockwise(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,LOW);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,HIGH);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
 
}


void stop()
{
        Serial.println("i am in stop");
  if(z>0)
  {
      z -=10;
  analogWrite(pwmLF,z);
  analogWrite(pwmRF,z);
  analogWrite(pwmLB,z);
  analogWrite(pwmRB,z);
      delay(100);
  }

  analogWrite(pwmLF,0);
  analogWrite(pwmRF,0);
  analogWrite(pwmLB,0);
  analogWrite(pwmRB,0);
} 

void Left(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,LOW);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,HIGH);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
 
}

void Right(float control)
{
  z = mapp(control);
  digitalWrite(DirLF,HIGH);
  analogWrite(pwmLF,z);
  digitalWrite(DirRF,LOW);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
 
}


void loop() {
  
  Controls();
    
 if (Ly > 30) {
    front(Ly);
    Serial.println(Ly);
  } else if (Ly < -30) {
        Serial.println(Ly);
    back(Ly);
  } else if (Lx < -30) {
        Serial.println(Ly);
    Right(Lx);
  } else if (Lx > 30) {
        Serial.println(Ly);
    Left(Lx);
  } else if (Rx > 30) {
        Serial.println(Ly);
    clockwise(Rx);
  } else if (Rx < -30) {
        Serial.println(Ly);
    anticlockwise(Rx);
  } else {
    stop();
  }

}