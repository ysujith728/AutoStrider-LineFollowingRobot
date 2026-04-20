
#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12

#define MOTOR_SPEED_RIGHT 300
#define MOTOR_SPEED_LEFT 300   

// Right motor pins
int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

// Left motor pins
int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;


// Function: rotateMotor
// Controls motor direction and speed

void rotateMotor(int rightMotorSpeed,int leftMotorSpeed)
{
  
  if(rightMotorSpeed<0)
  {
    digitalWrite(rightMotorPin1,HIGH);   // backward
    digitalWrite(rightMotorPin2,LOW);
  }
  else if(rightMotorSpeed>0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);   // forward
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);
  }

  
  if (leftMotorSpeed<0)
  {
    digitalWrite(leftMotorPin1,HIGH);    // backward
    digitalWrite(leftMotorPin2,LOW);
  }
  else if (leftMotorSpeed>0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    // forward
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);
  }

  // Apply PWM for speed control
  analogWrite(enableRightMotor,abs(rightMotorSpeed));
  analogWrite(enableLeftMotor,abs(leftMotorSpeed));
}

// ------------------------------------------------------------
// Setup: Runs once when Arduino starts
// ------------------------------------------------------------
void setup()
{
  // Change PWM frequency for smoother motor control
  TCCR0B=TCCR0B & B11111000 | B00000010; // ~7.8kHz

  // Set motor pins as outputs
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);

  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  // Set IR sensor pins as input
  pinMode(IR_SENSOR_RIGHT,INPUT);
  pinMode(IR_SENSOR_LEFT,INPUT);

  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("=====================================");
  Serial.println("AUTO STRIDER - LINE FOLLOWING ROBOT");
  Serial.println("=====================================");
  Serial.println("System Ready! Monitoring IR Sensors...");
  Serial.println();

  // Stop motors initially
  rotateMotor(0,0);
}

// ------------------------------------------------------------
// Loop: Runs continuously
// ------------------------------------------------------------
void loop()
{
  // Read IR sensor values
  int rightIRSensorValue=digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue=digitalRead(IR_SENSOR_LEFT);

  // Print raw sensor readings
  Serial.print("Right Sensor: ");
  Serial.print(rightIRSensorValue);
  Serial.print(" | Left Sensor: ");
  Serial.print(leftIRSensorValue);
  Serial.print(" --> ");

  // Decision logic for robot direction
  if (rightIRSensorValue==LOW && leftIRSensorValue==LOW)
  {
    // Both sensors on black line → move forward
    rotateMotor(MOTOR_SPEED_RIGHT,MOTOR_SPEED_LEFT);
    Serial.println("MOVING FORWARD");
  }
  else if (rightIRSensorValue==HIGH && leftIRSensorValue==LOW)
  {
    // Left on line, right off line → turn left
    rotateMotor(-MOTOR_SPEED_RIGHT,MOTOR_SPEED_LEFT);
    Serial.println("TURNING LEFT");
  }
  else if (rightIRSensorValue==LOW && leftIRSensorValue==HIGH)
  {
    // Right on line, left off line → turn right
    rotateMotor(MOTOR_SPEED_RIGHT, -MOTOR_SPEED_LEFT);
    Serial.println("TURNING RIGHT");
  }
  else
  {
    // Both off line → stop
    rotateMotor(0,0);
    Serial.println("STOPPED");
  }

  // Short delay for readability
  delay(300);
}

