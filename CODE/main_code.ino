#define three_sensors false
int centerSide = 90;
#include <Servo.h>
int m1a = 6;
int m1b = 11;
#define ECHOPIN 8
#define TRIGPIN 7
#define servo_pin 10
#define pos 90
#define echoL 2
#define trigL 3
#define trigR 12
#define echoR 13
int middleDistance, leftDistance, rightDistance, calcDistance, duration;
char val;
Servo steering;
Servo myservo;

int checkDistance()
{
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  unsigned long duratn = pulseIn(ECHOPIN, HIGH);
  float calcDist = ((float)duratn * 0.0343) / 2.0;
  calcDistance = (int)calcDist;
  delay(1);
  return calcDistance;
}

int checkDistanceR()
{
  digitalWrite(trigR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigR, LOW);
  unsigned long duratn = pulseIn(echoR, HIGH);
  float calcDist = ((float)duratn * 0.0343) / 2.0;
  calcDistance = (int)calcDist;
  delay(1);
  return calcDistance;
}

int checkDistanceL()
{
  digitalWrite(trigL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigL, LOW);
  unsigned long duratn = pulseIn(echoL, HIGH);
  float calcDist = ((float)duratn * 0.0343) / 2.0;
  calcDistance = (int)calcDist;
  delay(1);
  return calcDistance;
}

void forward() {
  digitalWrite(m1b, LOW);
  analogWrite(m1a, 80);
  steering.write(pos);
}

void backward() {
  digitalWrite(m1a, LOW);
  analogWrite(m1b, 80);
  steering.write(pos);
}

void left() {
  steering.write(179);
  digitalWrite(m1b, LOW);
  analogWrite(m1a, 80);
}

void right() {
  steering.write(0);
  analogWrite(m1a, 80);
  digitalWrite(m1b, LOW);
}

void Stop() {
  analogWrite(m1a, 0);
  digitalWrite(m1b, LOW);
  steering.write(pos);
}

void bluetooth()
{
  while (true) {
    while (Serial.available() > 0) {
      val = Serial.read();
      Serial.println(val);
    }
    if (val == 'F') {
      forward();
    }
    else if (val == 'B') {
      backward();
    }
    else if (val == 'L') {
      left();
    }
    else if (val == 'R') {
      right();
    }
    else if (val == 'S') {
      Stop();
    }
    else if (val == 'X') {
      Stop();
    }
    else if (val == 'x') {
      return;
    }
  }
}

void setup()
{
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  Serial.begin(9600);
  steering.attach(servo_pin);
  steering.write(pos);
  myservo.attach(9);
  myservo.write(centerSide);
}

void loop()
{
  while (Serial.available() > 0)
  {
    val = Serial.read();
    Serial.println(val);
    if (val == 'X') bluetooth();
  }
  middleDistance = checkDistance();
  runWithoutBluetooth();
}

void runWithoutBluetooth()
{
  if (middleDistance <= 50)
  {
    Stop();
    delay(200);
    backward();
    delay(1000);
    Stop();
    delay(10);
    int s = 0;
    if (three_sensors)
    {
      rightDistance = checkDistanceR();
      leftDistance = checkDistanceL();
    }
    else
    {
      for (int t = centerSide; t <= 170; t++)
      {
        myservo.write(t);
        delay(25);
      }
      leftDistance = checkDistance();
      Serial.println(leftDistance);
      for (int tt = 169; tt >= 0; tt--)
      {
        myservo.write(tt);
        delay(25);
      }
      rightDistance = checkDistance();
      Serial.println(rightDistance);
      for (int ttt = 1; ttt <= centerSide; ttt++)
      {
        myservo.write(ttt);
        delay(25);
      }
    }
    if (rightDistance > leftDistance)
    {
      right();
      Serial.println("right");
      delay(2000);
    }
    else if (rightDistance < leftDistance)
    {
      left();
      delay(2000);
      Serial.println("left");
    }
    else if ((rightDistance <= 50) && (leftDistance <= 50))
    {
      backward();
      delay(1000);
    }
  }
  else forward();
}
