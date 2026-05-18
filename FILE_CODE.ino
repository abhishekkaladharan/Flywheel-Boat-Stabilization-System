#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// ---------------- MOTOR PINS ----------------
#define STEP_PIN 2
#define DIR_PIN 3

// ---------------- USER PARAMETERS ----------------

// PD control gains
float Kp = 6.0;
float Kd = 1.2;

// RPM settings
float MIN_RPM = 40;
float MAX_RPM = 140;
float RPM_SCALE = 5.5;

// Stability dead zone
float DEAD_ANGLE = 3.5;
float DEAD_GYRO  = 3.5;

// Stepper settings
int stepsPerRev = 1600;

// Filter constants
float angleFilter = 0.90;
float gyroFilter  = 0.80;

// ---------------- VARIABLES ----------------

float roll = 0;
float filteredRoll = 0;

float gyroX = 0;
float filteredGyro = 0;

float zeroRoll = 0;
float gyroOffset = 0;

float error = 0;

unsigned long stepDelay;

// ---------------- CALIBRATION ----------------

void calibrateMPU() {

  float sumRoll = 0;
  float sumGyro = 0;

  Serial.println("Calibrating MPU6050...");
  Serial.println("Keep system stable");

  for (int i = 0; i < 200; i++) {

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);

    float tempRoll = atan2(ay, az) * 180 / PI;

    sumRoll += tempRoll;
    sumGyro += gx;

    delay(5);
  }

  zeroRoll = sumRoll / 200.0;
  gyroOffset = sumGyro / 200.0;

  Serial.println("Calibration Completed");
}

// ---------------- SETUP ----------------

void setup() {

  Wire.begin();
  Serial.begin(9600);

  mpu.initialize();

  if (!mpu.testConnection()) {

    Serial.println("MPU6050 connection failed");

    while (1);
  }

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  delay(2000);

  calibrateMPU();
}

// ---------------- LOOP ----------------

void loop() {

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  // Read MPU6050
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // ---------------- ROLL ANGLE ----------------

  float rawRoll = atan2(ay, az) * 180 / PI;
  rawRoll -= zeroRoll;

  // Low-pass filter
  filteredRoll =
      angleFilter * filteredRoll +
      (1.0 - angleFilter) * rawRoll;

  roll = filteredRoll;

  // ---------------- GYRO X ----------------

  float rawGyro = (gx - gyroOffset) / 131.0;

  // Gyro filtering
  filteredGyro =
      gyroFilter * filteredGyro +
      (1.0 - gyroFilter) * rawGyro;

  gyroX = filteredGyro;

  // ---------------- ERROR ----------------

  error = -roll;

  // ---------------- SERIAL MONITOR ----------------

  Serial.print("Roll: ");
  Serial.print(roll);

  Serial.print("  GyroX: ");
  Serial.print(gyroX);

  Serial.print("  Error: ");
  Serial.print(error);

  Serial.print("  RPM: ");

  float debugRPM =
      (abs(error) * Kp +
       abs(gyroX) * Kd) *
      RPM_SCALE;

  Serial.println(debugRPM);

  // ---------------- DEAD ZONE ----------------

  if (abs(error) < DEAD_ANGLE &&
      abs(gyroX) < DEAD_GYRO) {

    delay(5);
    return;
  }

  // ---------------- PD CONTROL ----------------

  float output =
      (Kp * error) +
      (Kd * gyroX);

  // ---------------- DIRECTION ----------------

  if (output > 0)
    digitalWrite(DIR_PIN, HIGH);
  else
    digitalWrite(DIR_PIN, LOW);

  // ---------------- RPM CONTROL ----------------

  float rpm = abs(output) * RPM_SCALE;

  // RPM limits
  if (rpm > MAX_RPM)
    rpm = MAX_RPM;

  if (rpm < MIN_RPM)
    rpm = MIN_RPM;

  // ---------------- STEP DELAY ----------------

  stepDelay =
      (60.0 * 1000000.0) /
      (rpm * stepsPerRev);

  // ---------------- MOTOR ROTATION ----------------

  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(stepDelay / 2);

  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(stepDelay / 2);
}