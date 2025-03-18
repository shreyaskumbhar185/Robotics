#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float gyroZ;
int yaw = 0;
float dt;
unsigned long currentTime, previousTime;
float gyroZ_bias = 0;
const float alpha = 0.98; 

void setup() {
    Serial.begin(115200);
    Wire.begin(15, 14); // Custom SDA (15) and SCL (14) for ESP32
    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed!");
        while (1);
    }

    Serial.println("MPU6050 is connected"); 

    calibrateGyro();
    previousTime = micros(); // More precise timing
}

void loop() {
    currentTime = micros();
    dt = (currentTime - previousTime) / 1000000.0; 
    previousTime = currentTime;
    
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    
    gyroZ = (gz / 131.0) - gyroZ_bias;  
   
    yaw += gyroZ * dt;

    // Wrap yaw to 0 - 360°
    if (yaw >= 360) yaw -= 360;
    if (yaw < 0) yaw += 360;

    Serial.print("Yaw: "); Serial.println(yaw);
    delay(10);
}

void calibrateGyro() {
    int numReadings = 1000;  
    float sum = 0;

    for (int i = 0; i < numReadings; i++) {
        int16_t gx, gy, gz;
        mpu.getRotation(&gx, &gy, &gz);
        sum += (gz / 131.0);
        delay(2);
    }
    
    gyroZ_bias = sum / numReadings;
    Serial.print("Gyro Z Bias: "); Serial.println(gyroZ_bias, 4);
}
