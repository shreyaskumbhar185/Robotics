#include <ps5Controller.h>

//DEFINING VARIABLES
int pwmLF=17,pwmRF=22,pwmLB=19,pwmRB=16;
int DirLF=18,DirRF=23,DirLB=21 ,DirRB=4;
// int pwmLF = 19, pwmRF = 17, pwmLB = 16, pwmRB = 22;   //not applicable because designing orietation cant be changed
// int DirLF = 21, DirRF = 18, DirLB = 4, DirRB = 23;
float Lx,Rx,Ly,Ry,z;
bool R1;

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
  R1=ps5.R1();
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
void Right(float control)   //front
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


void Left(float control)  //back
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

void Front(float control) //left
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
void RightFront(float control) //left front
{
  z = mapp(control);
  // digitalWrite(DirLF,LOW);
  // analogWrite(pwmLF,z);
  analogWrite(pwmLF,0);
  digitalWrite(DirRF,HIGH);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  analogWrite(pwmRB,0);
  // digitalWrite(DirRB,LOW);
  // analogWrite(pwmRB,z);
 
}
void LeftFront(float control)  //left back
{
  z = mapp(control);
  // digitalWrite(DirLF,LOW);
  // analogWrite(pwmLF,z);
  analogWrite(pwmLF,0);
  digitalWrite(DirRF,LOW);
  analogWrite(pwmRF,z);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  analogWrite(pwmRB,0);
  // digitalWrite(DirRB,LOW);
  // analogWrite(pwmRB,z);
 
}

void Back(float control) // back
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
void RightBack(float control)  //right front
{
  z = mapp(control);
  digitalWrite(DirLF,HIGH);
  analogWrite(pwmLF,z);
  analogWrite(pwmRF,0);
  analogWrite(pwmLB,0);
  // digitalWrite(DirRF,LOW);
  // analogWrite(pwmRF,z);
  // digitalWrite(DirLB,LOW);
  // analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
 
}
void LeftBack(float control) //right back
{
  z = mapp(control);
  digitalWrite(DirLF,LOW);
  analogWrite(pwmLF,z);
  analogWrite(pwmRF,0);
  analogWrite(pwmLB,0);
  // digitalWrite(DirRF,LOW);
  // analogWrite(pwmRF,z);
  // digitalWrite(DirLB,LOW);
  // analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
 
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
    Right(Lx);
  } 
  else if (Lx > 30) {
        Serial.println(Lx);
    Left(Lx);
  } 
  else if (Rx > 30) {
        Serial.println(Rx);
    clockwise(Rx);
  } 
  else if (Rx < -30) {
        Serial.println(Rx);
    anticlockwise(Rx);
  } 
  else if (Ry < -30) {
        Serial.println(Ry);
    if (R1){
      LeftFront(Ry);
    } else {
    LeftBack(Ry);
    }
  } 
  else if (Ry > 30) {
        Serial.println(Ry);
    if (R1){
      RightBack(Ry);
    } else {
      RightFront(Ry);
      }
  } 
  // else if (Ry < -30 && R1==1) {
  //       Serial.println(Ry);
  //   LeftFront(Ry);
  // } 
  // else if (Ry > 30 && R1==1) {
  //       Serial.println(Ry);
  //   RightBack(Ry);
  // } 
  else {
    stop();
  }

}
