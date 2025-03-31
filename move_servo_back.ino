#include <Servo.h>

// --- Servo Objects & Global Variables ---
Servo gripper;
Servo ax1;
Servo ax2;

// Communication pins for Arduino2 (Slave)
// These are swapped relative to Arduino1:
// Outgoing pins: 7 & 8; Incoming pins: 5 & 6.
const int comm_outPin0 = 7;  // sends bit0 to Arduino1 (wired from Arduino2 pin 7 -> Arduino1 pin 5)
const int comm_outPin1 = 8;  // sends bit1 to Arduino1 (wired from Arduino2 pin 8 -> Arduino1 pin 6)
const int comm_inPin0  = 5;  // reads bit0 from Arduino1 (wired from Arduino1 pin 5 -> Arduino2 pin 5)
const int comm_inPin1  = 6;  // reads bit1 from Arduino1 (wired from Arduino1 pin 6 -> Arduino2 pin 6)

// Gripper positions
int gr_open = 40;
int gr_close = 100;

// Home positions (servo angles in degrees)
const float HOME_AX1 = 40;
const float HOME_AX2 = 108;
float ax1_current = HOME_AX1;
float ax2_current = HOME_AX2;

// Hardcoded target positions (servo angles) for each target:
// Target 1:
const float t1ax1 = 40;
const float t1ax2 = 108;
// Target 2:
const float t2ax1 = 60;
const float t2ax2 = 90;
// Target 3:
const float t3ax1 = 80;
const float t3ax2 = 70;

void moveTopos(float start1, float end1, float start2, float end2) {
  float diff1 = end1 - start1;
  float diff2 = end2 - start2;
  int steps = max(abs(diff1), abs(diff2));
  if (steps == 0) steps = 1;
  float inc1 = diff1 / steps;
  float inc2 = diff2 / steps;
  for (int i = 0; i <= steps; i++) {
    float pos1 = start1 + inc1 * i;
    float pos2 = start2 + inc2 * i;
    ax1.write(pos1);
    ax2.write(pos2);
    delay(25);
  }
  ax1_current = end1;
  ax2_current = end2;
}

// Move to a position given by two servo angles.
void moveToPosition(float targetAx1, float targetAx2) {
  moveTopos(ax1_current, targetAx1, ax2_current, targetAx2);
}

// --- Communication Functions ---
// Broadcast a target state using 2 bits.
// Mapping: Target 1 = 00, Target 2 = 01, Target 3 = 10.
void setTargetState(int target) {
  int bit0 = 0;
  int bit1 = 0;
  switch (target) {
    case 1: bit0 = 0; bit1 = 0; break;
    case 2: bit0 = 1; bit1 = 0; break;
    case 3: bit0 = 0; bit1 = 1; break;
    default: bit0 = 0; bit1 = 0; break;
  }
  digitalWrite(comm_outPin0, bit0);
  digitalWrite(comm_outPin1, bit1);
}

// Read the 2‑bit state coming from Arduino1.
int readTargetState() {
  int b0 = digitalRead(comm_inPin0);
  int b1 = digitalRead(comm_inPin1);

  if (b0 == 0 && b1 == 0) return 1;
  if (b0 == 1 && b1 == 0) return 2;
  if (b0 == 0 && b1 == 1) return 3;
  return 0;
}

// --- Setup ---
void setup() {
  Serial.begin(9600);
  gripper.attach(9);
  ax1.attach(10);
  ax2.attach(11);

  pinMode(comm_outPin0, OUTPUT);
  pinMode(comm_outPin1, OUTPUT);
  pinMode(comm_inPin0, INPUT);
  pinMode(comm_inPin1, INPUT);

  ax1_current = HOME_AX1;
  ax2_current = HOME_AX2;
  ax1.write(ax1_current);
  ax2.write(ax2_current);
  gripper.write(gr_open);
  
  // Initialize with a default state (Target 1)
  setTargetState(1);
  
  Serial.println("Slave Arduino Initialized");
  Serial.println("Waiting for current object state from Master...");
}

// --- Main Loop ---
void loop() {
  int sourceState = readTargetState();
  
  Serial.print("Current object location (source state): Target ");
  Serial.println(sourceState);

  Serial.println("Enter drop-off target number:");

  while (Serial.available() == 0) {}
  int dropTarget = Serial.parseInt();
  Serial.print("Selected drop-off target: ");
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
  
  Serial.println("Moving to pickup position...");
  moveToPosition(pickupAx1, pickupAx2);
  delay(500);
  
  Serial.println("Picking up object...");
  gripper.write(gr_close);
  delay(1000);
  
  Serial.println("Moving to drop-off position...");
  moveToPosition(dropAx1, dropAx2);
  delay(500);
  
  Serial.println("Dropping object...");
  gripper.write(gr_open);
  delay(1000);
  
  Serial.println("Returning to home position...");
  moveToPosition(HOME_AX1, HOME_AX2);
  delay(500);
  
  setTargetState(dropTarget);
  Serial.print("New object location set to Target ");
  Serial.println(dropTarget);
  
  delay(3000);
}