// ===========================
// PID Line Follower + Turn Detection
// ===========================

// --- Constants ---
#define SENSOR_NUMBER 8
#define THRESHOLD_VAL 100  // >100 = black

// --- IR Sensor Pins ---
const uint8_t irPins[SENSOR_NUMBER] = {A0, A1, A2, A3, A4, A5, A6, A7};

// --- Motor Pins ---
#define ENA 10  // Left motor PWM
#define IN1 9
#define IN2 8

#define ENB 5   // Right motor PWM
#define IN3 7
#define IN4 6

// --- Ultrasonic Sensor ---
#define TRIG 3
#define ECHO 4
#define OBSTACLE_DISTANCE 15   // cm
bool obstacleDetected = false;

// --- PID Variables ---
float linePosition = 0;
float error = 0;
float previousError = 0;
float derivative = 0;
float centerPosition = 45;

// --- PID Tuning ---
float Kp = 43;
float Kd = 120;
float Ki = 0.019;

// --- Motor Speed ---
int baseSpeed = 130;

// --- Sensor Variables ---
int sensorADC[SENSOR_NUMBER];
int sensorDigital[SENSOR_NUMBER];
int bitWeight[SENSOR_NUMBER]   = {1, 2, 4, 8, 16, 32, 64, 128};
int WeightValue[SENSOR_NUMBER] ={80, 70, 60, 50, 40, 30, 20, 10};
int sumOnSensor = 0;
int sensorWeight = 0;
int bitSensor = 0;

// --- Turn logic variables ---
String direction = "straight"; 
#define delay_before_turn 30
#define turnSpeed 45

// ===========================
// SETUP
// ===========================
void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("=== PID + TURN Line Follower Initialized ===");
  delay(1000);
}

// ===========================
// LOOP
// ===========================
void loop() {

  checkObstacle();
  if (obstacleDetected) {
    stopMotors();
    return;
  }

  readSensors();
  detectTurnPattern();
  handleTurns();
  PID_Controller();
  delay(10);
}

// ===========================
// ULTRASONIC FUNCTION
// ===========================
void checkObstacle() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000);
  int distance = duration * 0.034 / 2;

  if (distance > 0 && distance < OBSTACLE_DISTANCE)
    obstacleDetected = true;
  else
    obstacleDetected = false;
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ===========================
// SENSOR FUNCTIONS
// ===========================
void readSensors() {
  sumOnSensor = 0;
  sensorWeight = 0;
  bitSensor = 0;

  for (int i = 0; i < SENSOR_NUMBER; i++) {
    sensorADC[i] = analogRead(irPins[i]);
    sensorDigital[i] = (sensorADC[i] > THRESHOLD_VAL) ? 1 : 0;
    sumOnSensor += sensorDigital[i];
    sensorWeight += sensorDigital[i] * WeightValue[i];
    bitSensor |= (sensorDigital[i] << i);
  }

  Serial.print("bitSensor: ");
  Serial.println(bitSensor, BIN);

  if (sumOnSensor > 0)
    linePosition = sensorWeight / sumOnSensor;
  else
    linePosition = -1;
}

// ===========================
// TURN PATTERN DETECTION
// ===========================
void detectTurnPattern() {
  switch(bitSensor) {
    case 0b11111000:
    case 0b11110000:
    case 0b11111100:
      direction = "left";
      break;

    case 0b00011111:
    case 0b00001111:
    case 0b00111111:
      direction = "right";
      break;
  }
}

// ===========================
// HANDLE TURNS
// ===========================
void handleTurns() {

  if (bitSensor != 0) return;
  readSensors();
  if (bitSensor != 0) return;
  if (direction == "straight") return;

  delay(delay_before_turn);

  if (direction == "right") turnRight(turnSpeed, turnSpeed);
  else turnLeft(turnSpeed, turnSpeed);

  direction = "straight";
}

// ===========================
// TURN FUNCTIONS
// ===========================
void turnRight(int L, int R) {
  while (sensorDigital[3] && sensorDigital[4] != 1) {
    setMotorSpeed(L, -R);
    readSensors();
  }
}

void turnLeft(int L, int R) {
  while (sensorDigital[3] && sensorDigital[4] != 1) {
    setMotorSpeed(-L, R);
    readSensors();
  }
}

// ===========================
// MOTOR FUNCTIONS
// ===========================
void setMotorSpeed(int LPWM, int RPWM) {
  if (LPWM >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else           { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); }

  if (RPWM >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else           { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); }

  LPWM = constrain(LPWM, -255, 255);
  RPWM = constrain(RPWM, -255, 255);

  analogWrite(ENA, abs(LPWM));
  analogWrite(ENB, abs(RPWM));
}

// ===========================
// PID CONTROLLER
// ===========================
void PID_Controller() {

  error = centerPosition - linePosition;
  derivative = error - previousError;

  int correction = (Kp * error) + (Kd * derivative);
  correction = constrain(correction, -baseSpeed, baseSpeed);
  previousError = error;

  int leftSpeed  = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;

  leftSpeed  = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  setMotorSpeed(leftSpeed, rightSpeed);

  Serial.print("Err: "); Serial.print(error);
  Serial.print(" | L: "); Serial.print(leftSpeed);
  Serial.print(" | R: "); Serial.print(rightSpeed);
  Serial.print(" | Pos: "); Serial.println(linePosition);
}