#include <Servo.h>

Servo gripper;
Servo ax1;
Servo ax2;

const int comm_outPin0 = 5;
const int comm_outPin1 = 6;
const int comm_inPin0  = 7;
const int comm_inPin1  = 8;

int gr_open = 40;
int gr_close = 100;

const float HOME_AX1 = 40;
const float HOME_AX2 = 108;
int digitOne;
int digitTwo;
float ax1_current = HOME_AX1;
float ax2_current = HOME_AX2;

const float t1ax1 = 40;
const float t1ax2 = 108;
const float t2ax1 = 60;
const float t2ax2 = 90;
const float t3ax1 = 80;
const float t3ax2 = 70;

void moveTopos(float ax1s, float ax1e, float ax2s, float ax2e) {
  float num_steps_1 = ax1e - ax1s;
  float num_steps_2 = ax2e - ax2s;
  float num_steps = 0;
  if (abs(num_steps_1) >= abs(num_steps_2)) {
    num_steps = abs(num_steps_1);
  } else {
    num_steps = abs(num_steps_2);
  }
  float inc1 = num_steps_1 / num_steps;
  float inc2 = num_steps_2 / num_steps;
  for (float stp = 0; stp <= num_steps; stp += 1) {
    ax1_current += inc1;
    ax1.write(ax1_current);
    ax2_current += inc2;
    ax2.write(ax2_current);
    delay(25);
  }
  ax1_current = ax1e;
  ax1.write(ax1_current);
  ax2_current = ax2e;
  ax2.write(ax2_current);
  delay(25);
}



int updateInternalPos(int x, int y){
  if (x==0 && y==1){
    return 1;
  }
  if (x==1 && y==0){
    return 2;
  }
  if (x==1 && y==1){
    return 3;
  }
}

void setTargetState(int target) {
  int bit0 = 0;
  int bit1 = 0;

  switch (target) {
    case 1: bit0 = 0; bit1 = 1; break;
    case 2: bit0 = 1; bit1 = 0; break;
    case 3: bit0 = 1; bit1 = 1; break;
    default: bit0 = 0; bit1 = 0; break;
  }

  digitalWrite(comm_outPin0, bit0);
  digitalWrite(comm_outPin1, bit1);
}

void setup() {
  Serial.begin(9600);
  gripper.attach(10);

  ax1.attach(11);
  ax2.attach(12);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  ax1_current = HOME_AX1;
  ax2_current = HOME_AX2;

  ax1.write(ax1_current);
  ax2.write(ax2_current);
  
  gripper.write(gr_open);
  Serial.println("Front Arduino");

  digitalWrite(5, 0);
  digitalWrite(6, 1);
}


void loop() {
  digitOne = digitalRead(7);
  digitTwo = digitalRead(8);
  digitalWrite(5, digitOne);
  digitalWrite(6, digitTwo);
  int sourceState = updateInternalPos(digitOne, digitTwo);

  if (Serial.available() >0 ) {

  int dropTarget = Serial.parseInt();
  Serial.print("Selected drop off location: ");
  Serial.println(dropTarget);
  float pickupAx1, pickupAx2, dropAx1, dropAx2;
  if (sourceState == 1) { 
    pickupAx1 = t1ax1; 
    pickupAx2 = t1ax2; 
  } else if (sourceState == 2) { 
    pickupAx1 = t2ax1; 
    pickupAx2 = t2ax2; 
  } else if (sourceState == 3) { 
    pickupAx1 = t3ax1; 
    pickupAx2 = t3ax2; 
  }
  if (dropTarget == 1) { 
    dropAx1 = t1ax1; 
    dropAx2 = t1ax2; 
  } else if (dropTarget == 2) { 
    dropAx1 = t2ax1; 
    dropAx2 = t2ax2; 
  } else if (dropTarget == 3) { 
    dropAx1 = t3ax1; 
    dropAx2 = t3ax2; 
  }
  
  Serial.println("Moving to pickup location");
  moveTopos(ax1_current, pickupAx1, ax2_current, pickupAx2);
  delay(500);
  Serial.println("Picking up object");
  gripper.write(gr_close);
  delay(1000);
  Serial.println("Moving to drop off location");
  moveTopos(ax1_current, dropAx1, ax2_current, dropAx2);
  delay(500);
  Serial.println("Dropping object");
  gripper.write(gr_open);
  delay(1000);
  Serial.println("Returning to home location");
  moveTopos(ax1_current, HOME_AX1, ax2_current, HOME_AX2);
  delay(500);
  setTargetState(dropTarget);
  Serial.print("New object location set to Target ");
  Serial.println(dropTarget);
  delay(3000);
    }
}