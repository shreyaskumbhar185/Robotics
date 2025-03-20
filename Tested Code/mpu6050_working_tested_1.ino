#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

//   MPU control/status vars
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// Orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// Timers
unsigned long timer = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to be ready - comment this line for standalone operation
    
    // Initialize device
    Serial.println(F("Initializing I2C devices..."));
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock
    delay(100);
    
    // Initialize MPU6050
    Serial.println(F("Initializing MPU6050..."));
    mpu.initialize();
    
    // Verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    
    // Load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    
    // Make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        
        // Get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
        
        // Set our DMP Ready flag so the main loop() function knows it's ready
        Serial.println(F("DMP ready!"));
        dmpReady = true;
        
        // Calibration - Perform self-test and get offsets
        Serial.println(F("Calibrating - please keep the sensor still..."));
        
        // Set gyro and accel offsets to sensible starting values
        mpu.setXGyroOffset(0);
        mpu.setYGyroOffset(0);
        mpu.setZGyroOffset(0);
        mpu.setXAccelOffset(0);
        mpu.setYAccelOffset(0);
        mpu.setZAccelOffset(0);
        
        // Then calibrate
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        
        Serial.println(F("Calibration complete!"));
        Serial.println(F("Starting data output..."));
        delay(1000);
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

void loop() {
    // If programming failed, don't try to do anything
    if (!dmpReady) {
        Serial.println("DMP not ready!");
        delay(1000);
        return;
    }
    
    // This approach uses polling instead of interrupts
    // Reset FIFO
    mpu.resetFIFO();
    
    // Wait for FIFO to fill with fresh data
    delay(50);
    
    // Get current FIFO count
    fifoCount = mpu.getFIFOCount();
    
    if (fifoCount < packetSize) {
        // Not enough data yet
        Serial.println("Waiting for data...");
        delay(10);
        return;
    }
    
    if (fifoCount >= 1024) {
        // FIFO overflow - reset and try again
        Serial.println(F("FIFO overflow!"));
        mpu.resetFIFO();
        return;
    }
    
    // Read a packet from FIFO
    if (fifoCount >= packetSize) {
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // Get Euler angles and acceleration
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        
        // Print data every 200ms
        if (millis() - timer > 200) {
            // Convert yaw/pitch/roll angles from radians to degrees
            float yaw = ypr[0] * 180 / M_PI;
            float pitch = ypr[1] * 180 / M_PI;
            float roll = ypr[2] * 180 / M_PI;
            
            Serial.println("=======================================================");
            
            // Print angles in degrees
            Serial.print("ANGLE: ");
            Serial.print("Yaw = ");
            Serial.print(yaw);
            Serial.print("\tPitch = ");
            Serial.print(pitch);
            Serial.print("\tRoll = ");
            Serial.println(roll);
            
            // Print raw acceleration (in g's)
            float accelX = (float)aa.x / 16384.0;  // Convert to g (±2g range => 16384 LSB/g)
            float accelY = (float)aa.y / 16384.0;
            float accelZ = (float)aa.z / 16384.0;
            
            Serial.print("RAW ACCELERATION (g): ");
            Serial.print("X = ");
            Serial.print(accelX);
            Serial.print("\tY = ");
            Serial.print(accelY);
            Serial.print("\tZ = ");
            Serial.println(accelZ);
            
            // Print gravity-compensated acceleration (in g's)
            float realAccelX = (float)aaReal.x / 16384.0;
            float realAccelY = (float)aaReal.y / 16384.0;
            float realAccelZ = (float)aaReal.z / 16384.0;
            
            Serial.print("GRAVITY-FREE ACCELERATION (g): ");
            Serial.print("X = ");
            Serial.print(realAccelX);
            Serial.print("\tY = ");
            Serial.print(realAccelY);
            Serial.print("\tZ = ");
            Serial.println(realAccelZ);
            
            // Print gravity-compensated and rotated acceleration (in g's)
            float worldAccelX = (float)aaWorld.x / 16384.0;
            float worldAccelY = (float)aaWorld.y / 16384.0;
            float worldAccelZ = (float)aaWorld.z / 16384.0;
            
            Serial.print("WORLD ACCELERATION (g): ");
            Serial.print("X = ");
            Serial.print(worldAccelX);
            Serial.print("\tY = ");
            Serial.print(worldAccelY);
            Serial.print("\tZ = ");
            Serial.println(worldAccelZ);
            
            // Print gravity-compensated and rotated acceleration (in m/s²)
            const float GRAVITY = 9.81;
            Serial.print("WORLD ACCELERATION (m/s²): ");
            Serial.print("X = ");
            Serial.print(worldAccelX * GRAVITY);
            Serial.print("\tY = ");
            Serial.print(worldAccelY * GRAVITY);
            Serial.print("\tZ = ");
            Serial.println(worldAccelZ * GRAVITY);
            
            Serial.println("=======================================================\n");
            timer = millis();
        }
    }
}
