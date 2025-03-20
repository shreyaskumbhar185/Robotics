#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

float pitch = 0, roll = 0,yaw=0; // Angles

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("MPU6050 Not Connected...");
    while (1);
  }
  Serial.println("MPU6050 Connected!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Convert Acceleration to Angle
  float accPitch = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float accRoll = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;

  // Gyro Angle Change
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // Convert to seconds
  lastTime = currentTime;

  float gyroPitch = g.gyro.x * dt;
  float gyroRoll = g.gyro.y * dt;

  // Complementary Filter: Combine Acceleration and Gyro Data
  float alpha = 0.98; // Adjust this for better results
  pitch = alpha * (pitch + gyroPitch) + (1 - alpha) * accPitch;
  roll = alpha * (roll + gyroRoll) + (1 - alpha) * accRoll;

  // Print the filtered angles
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.println(roll);

  delay(10); // Short delay for smoother readings
}
