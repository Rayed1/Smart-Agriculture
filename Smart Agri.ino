#include <Servo.h>

#define echoPin 8            // Ultrasonic sensor Echo pin
#define trigPin 7            // Ultrasonic sensor Trigger pin
#define motor 5              // Relay module pin for controlling the water pump
Servo myservo;              // Servo motor object for robotic movement

int msensor = A1;           // Soil moisture sensor pin
int msvalue = 0;            // Variable to store the moisture sensor value
long duration;              // Variable for the duration of the ultrasonic pulse
float moisture_percentage;  // Variable to calculate moisture percentage

void setup() {
  // Setup pins for sensors and motors
  pinMode(msensor, INPUT);  // Soil moisture sensor input
  pinMode(motor, OUTPUT);   // Relay output for water pump control
  pinMode(13, OUTPUT);      // Left motors forward
  pinMode(12, OUTPUT);      // Left motors reverse
  pinMode(11, OUTPUT);      // Right motors forward
  pinMode(10, OUTPUT);      // Right motors reverse

  myservo.attach(9);        // Attach the servo motor to pin 9
  pinMode(trigPin, OUTPUT); // Set the trigger pin as an output
  pinMode(echoPin, INPUT);  // Set the echo pin as an input
  Serial.begin(9600);       // Start serial communication at 9600 baud
}

void loop() {
  if (Serial.available()) {
    char t = Serial.read(); // Read the serial command

    // Read the soil moisture sensor value and convert it to percentage
    msvalue = analogRead(msensor);
    moisture_percentage = (100 - (msvalue / 1023.00) * 100);
    Serial.print("Moisture Percentage: ");
    Serial.println(moisture_percentage);

    // If the soil is too dry (below 5% moisture), turn on the water pump
    if (moisture_percentage < 5) {
      digitalWrite(motor, LOW); // Turn on the water pump
    }

    // If the soil is in an acceptable range (7% to 35%), stop the water pump
    if (moisture_percentage >= 7 && moisture_percentage <= 35) {
      digitalWrite(motor, HIGH); // Turn off the water pump
    }

    // Command 'B' to move forward (all motors rotate in forward direction)
    if (t == 'B') {
      digitalWrite(13, HIGH);  // Left motors forward
      digitalWrite(11, HIGH);  // Right motors forward
    }
    // Command 'X' to rotate the servo motor to 0 degrees (forward)
    else if (t == 'X') {
      myservo.write(0);        // Servo 0-180 degree
      delay(500);
    }
    // Command 'Y' to rotate the servo motor to 180 degrees (reverse)
    else if (t == 'Y') {
      myservo.write(180);      // Servo 180-0 degree
      delay(500);
    }
    // Command 'F' to move the robot in reverse if distance > 8 cm
    else if (t == 'F' && distance > 8) {
      digitalWrite(12, HIGH);  // Left motors reverse
      digitalWrite(10, HIGH);  // Right motors reverse
    }
    // Command 'L' to turn left (left side motors forward, right side doesn't rotate)
    else if (t == 'L') {
      digitalWrite(12, HIGH);  // Left motors forward
    }
    // Command 'R' to turn right (right side motors forward, left side doesn't rotate)
    else if (t == 'R') {
      digitalWrite(10, HIGH);  // Right motors forward
    }
    // Command 'S' to stop all motors
    else if (t == 'S') {
      digitalWrite(13, LOW);   // Left motors stop
      digitalWrite(12, LOW);   // Left motors stop
      digitalWrite(11, LOW);   // Right motors stop
      digitalWrite(10, LOW);   // Right motors stop
    }

    // Measure distance using the ultrasonic sensor
    if (distance < 15) {
      digitalWrite(12, LOW);   // Stop left motors
      digitalWrite(10, LOW);   // Stop right motors

      // Trigger the ultrasonic sensor to measure distance
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;  // Calculate the distance in cm
      Serial.println(distance);
    }
  }
  delay(50); // Short delay before the next iteration
}

