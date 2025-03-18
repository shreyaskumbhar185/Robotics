//INCLUDING LIBRARIES
#include <ps5Controller.h>
//DEFINING VARIABLES
int pwmLB=21,pwmRB=18,DirLB=19,DirRB=17;
float Lx,Rx,Ly,Ry,z;
//SETUP
void setup() {
  Serial.begin(115200);
  ps5.begin("BC:C7:46:0A:66:DA"); //replace with MAC address of your controller
 //TRY CONNECTING TO PS5 CONTROLLER
    while (!ps5.isConnected()) {
    Serial.println("Connecting...");
    }    
  Serial.println("Ready.");
  
  pinMode(pwmLB,OUTPUT);
  pinMode(pwmRB,OUTPUT);

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

void front(float control)
{

  z = mapp(control);
  
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
}

void back(float control)
{
  z = mapp(control);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
}

void clockwise(float control)
{
  z = mapp(control);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z);
}

void anticlockwise(float control)
{

  z = mapp(control);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z);
}

void stop()
{
      Serial.println("i am in stop");
  if(z>0)
  {
      z -=100;
      //digitalWrite(DirLB,HIGH);
      analogWrite(pwmLB,z);
      //digitalWrite(DirRB,HIGH);
      analogWrite(pwmRB,z);
      delay(50);
  }
  analogWrite(pwmLB,0);
  analogWrite(pwmRB,0);
} 

void Left(float control)
{

  z = mapp(control);
  digitalWrite(DirLB,LOW);
  analogWrite(pwmLB,z*0.8);
  digitalWrite(DirRB,HIGH);
  analogWrite(pwmRB,z*0.8);
}

void Right(float control)
{

  z = mapp(control);
  digitalWrite(DirLB,HIGH);
  analogWrite(pwmLB,z*0.8);
  digitalWrite(DirRB,LOW);
  analogWrite(pwmRB,z*0.8);
}

void loop() {
  
  Controls();
    
 if (Ly < -30) {
    front(Ly);
    Serial.println(Ly);
  } else if (Ly > 30) {
        Serial.println(Ly);
    back(Ly);
  } else if (Lx > 30) {
        Serial.println(Ly);
    Right(Lx);
  } else if (Lx < -30) {
        Serial.println(Ly);
    Left(Lx);
  } else if (Rx < -30) {
        Serial.println(Ly);
    clockwise(Rx);
  } else if (Rx > 30) {
        Serial.println(Ly);
    anticlockwise(Rx);
  } else {
    stop();
  }

}