#include <AFMotor.h>
#include <Servo.h>

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

// Servo motor on pin 3
Servo lidServo;
const int servoPin = 3;

// DC Motor on M1 (using motor shield)
AF_DCMotor motor(1); // Motor connected to M1

// Distance threshold
const int thresholdDistance = 20; // cm

void setup() {
  Serial.begin(9600);

  // Setup ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup servo
  lidServo.attach(servoPin);
  lidServo.write(0); // Start with lid closed

  // Setup motor
  motor.setSpeed(150); // Speed from 0–255
  motor.run(RELEASE);  // Ensure motor is stopped at startup
}

void loop() {
  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance <= thresholdDistance) {
    // Open lid
    lidServo.write(90); // Adjust as needed
    delay(500);

    // Move backward
    motor.setSpeed(150);
    motor.run(BACKWARD);
    delay(1500); // Move ~15cm (adjust after testing)
    motor.run(RELEASE); // Stop motor

    // Close lid
    lidServo.write(0);
    delay(2000); // Wait before checking again
  }

  delay(200);
}


// Function to measure distance
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout at 30ms
  if (duration == 0) return -1; // Return -1 if no response

  float distance = (duration * 0.0343) / 2.0;
  return distance;
}