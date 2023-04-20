// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Flywheel             motor_group   8, 17           
// MotorsR              motor_group   20, 16          
// MotorsL              motor_group   19, 18          
// Finger               motor         11              
// Inertial             inertial      14              
// Vision               vision        13              
// Controller1          controller                    
// Distance             distance      2               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Flywheel             motor_group   8, 17
// MotorsR              motor_group   20, 16
// MotorsL              motor_group   19, 18
// Finger               motor         11
// Inertial             inertial      14
// Vision               vision        13
// Controller1          controller
// Distance             distance      2
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\parkrail211                                      */
/*    Created:      Tue Mar 28 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Flywheel             motor_group   8, 17
// MotorsR              motor_group   20, 16
// MotorsL              motor_group   19, 18
// Finger               motor         11
// Inertial             inertial      14
// Vision               vision        13
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

bool rollingOut = true;
bool targetLocked = false;
float margin = 1;
int maxTurnSpeed = 25;
float Kp = 1;
float Ki = 0.0000000000001;


float map(float inputValue, float a1, float a2, float b1, float b2) {
  float output = b1 + (((inputValue - a1) * (b2 - b1)) / (a2 - a1));
  return output;
  // maps one value in a range to its corresponding value in another range
}

float VisionMid(vex::vision::signature sig) {
  Vision.takeSnapshot(sig);
  for (int i = 0; i < Vision.objectCount; i++) {
    float mid = Vision.objects[i].centerX;
    for (int j = 0; j < Vision.objectCount; j++) {
      if(Vision.objects[i].centerX < mid + margin && Vision.objects[i].centerX > mid - margin && i != j) {
        return map(mid, 315, 0, -100, 100);
      }
    }
  }
  return 101;
}

void targeting(vex::vision::signature sig) {
  targetLocked = false;
  if (!targetLocked) {
    float integral = 0;
    MotorsR.spin(forward);
    MotorsL.spin(forward);

    while (!(VisionMid(sig) < 0 + margin && VisionMid(sig) > 0 - margin)) {
      Vision.takeSnapshot(sig);
      float error = 0 - VisionMid(sig);
      integral = integral + error;

      if (error == 0) {
        integral = 0;
      }

      float speed = Kp * error;
      speed =
          map(speed, -100, 100, -maxTurnSpeed, maxTurnSpeed) + Ki * integral;

      MotorsR.setVelocity(speed, percent);
      MotorsL.setVelocity(-speed, percent);
    }
    targetLocked = true;
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("distance: %f", Distance.objectDistance(inches)/12);
    
    MotorsL.stop();
    MotorsR.stop();
  }
}

void shoot(float speed) {

  // converts distance from goal to how fast the flywheel needs to spin
  Flywheel.setVelocity(speed, percent);
  Flywheel.spin(forward);
  vex::task::sleep(2000);
  // spin up time
  Finger.setVelocity(50, percent);
  Finger.spin(forward);
  vex::task::sleep(3000);
  // pushes disks into flywheel
  Finger.stop();
  Flywheel.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Inertial.calibrate();

  while (rollingOut) {

    if (Controller1.ButtonA.pressing()) {

      MotorsL.setVelocity(10, percent);
      MotorsR.setVelocity(10,percent);
      
      MotorsL.spinFor(forward, 4,turns, false);
      MotorsR.spinFor(forward, 4, turns, true);

      targeting(Vision__GOAL);

      while (!(Distance.objectDistance(inches) < 4.5 && Distance.objectDistance(inches) > 3.5)) {
        
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print("Distance: %f",
                                 Distance.objectDistance(inches) / 12);
        float inch = Distance.objectDistance(inches);
        MotorsL.setVelocity(10, percent);
        MotorsR.setVelocity(10, percent);
        if (inch < 48) {
          MotorsL.spin(reverse);
          MotorsR.spin(reverse);

        } else {
          MotorsL.spin(forward);
          MotorsR.spin(forward);
        }
      }
      MotorsL.stop();
      MotorsR.stop();
      shoot(75);
    }
  }
}
