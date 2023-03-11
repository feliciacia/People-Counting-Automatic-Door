#include <Servo.h>
Servo myservo;

int sensitivity = 5;

int sensor1[] = {4,5};
int sensor2[] = {6,7};
int sensor1init;
int sensor2init;

String seq = "";
int time_count = 0;
int currentPeople = 0;
int buzzer = A0;
int maxPeople = 2;

void setup() {
  myservo.attach(9);
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);

  delay(500);
  sensor1init = measureDistance(sensor2);
  sensor2init = measureDistance(sensor2);
  Serial.print("sensor 1 init: ");
  Serial.println(sensor1init);
  Serial.print("sensor 2 init: ");
  Serial.println(sensor2init);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Read ultrasonic sensors
  int sensor1Val = measureDistance(sensor1);
  int sensor2Val = measureDistance(sensor2);
  //Serial.print("sensor 1 val: ");
  //Serial.println(sensor1Val);
  //Serial.print("sensor 2 val: ");
  //Serial.println(sensor2Val);
  
  //Process the data
  if(sensor1Val < 13 && sensor1Val > 3 && seq.charAt(0) != '1'){
    seq += "1";
    myservo.write(0);
    delay(5000);
    myservo.write(180);
    //delay(2000);
  }else if(sensor2Val < 15 && sensor1Val > 3 && seq.charAt(0) != '2'){
    seq += "2";
    myservo.write(0);
    delay(5000);
    myservo.write(180);
    //delay(2000);
  }
  
  if(seq.equals("12")){
    currentPeople++;  
    seq="";
    delay(200);
    myservo.write(0);
    delay(2000);
  }
  else if(seq.equals("21") && currentPeople > 0){
    currentPeople--;  
    seq="";
    delay(200);
    myservo.write(0);
    delay(2000);
  }
  
  Serial.print("People: ");
  Serial.println(currentPeople);
  Serial.print("\n");
  
  //Resets the sequence if it is invalid or timeouts
  if(seq.length() > 2 || seq.equals("11") || seq.equals("22") || time_count > 35){
    seq="";  
  }

  if(seq.length() == 1){ //
    time_count++;
    if(time_count > 25){
      tone(buzzer, 1500);
      Serial.print("buzzer on\n");
    }
  }else{
    time_count=0;
    noTone(buzzer);  
  }
  
  //Print values to serial
  Serial.print("Seq: ");
  Serial.print(seq);
  Serial.print("\ntime count: ");
  Serial.print(time_count);
  Serial.print("\n");
  Serial.print(" S1: ");
  Serial.print(sensor1Val);
  Serial.print(" S2: ");
  Serial.println(sensor2Val);
  
  //If the number of people is too high, trigger the buzzer
  if(currentPeople > maxPeople){
    //tone(buzzer, 1500);  
  }else{
    //noTone(buzzer);  
  }

  delay(200);
}

//a[0] = echo, a[1] = trig
int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  digitalWrite(a[1], LOW);
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  pinMode(a[0], INPUT);
  long duration = pulseIn(a[0], HIGH, 100000);
  //long duration = pulseIn(a[0], HIGH);
  //int distance = duration * 0.034 / 2;
  //return distance;
  return duration / 29 / 2;
}
