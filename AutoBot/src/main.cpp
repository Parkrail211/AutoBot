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
// ---- END VEXCODE CONFIGURED DEVICES ----
// Autobots, roll out!

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
float margin = 0.5;
int maxTurnSpeed = 25;
float Kp = 1;
float Ki = 0.000000025;
int midAdjust = -3;

float map(float inputValue, float a1, float a2, float b1, float b2) {
  float output = b1 + (((inputValue - a1) * (b2 - b1)) / (a2 - a1));
  return output;
  // maps one value in a range to its corresponding value in another range
}
float VisionMid(vex::vision::signature sig) {
  Vision.takeSnapshot(sig);
  float output = map(Vision.largestObject.centerX, 0, 315, 100, -100);
  return output;
}

void targeting(vex::vision::signature sig) {

  if (!targetLocked) {
    float integral = 0;
    MotorsR.spin(forward);
    MotorsL.spin(forward);

    while (!(VisionMid(sig) < midAdjust + margin && VisionMid(sig) > midAdjust - margin)) {
      Vision.takeSnapshot(sig);
      float error = 0 - VisionMid(sig);
      integral = integral + error;

      if (error == 0) {
        integral = 0;
      }

      float speed = Kp * error + Ki * integral;
      speed = map(speed, -100, 100, -maxTurnSpeed, maxTurnSpeed);

      MotorsR.setVelocity(speed, percent);
      MotorsL.setVelocity(-speed, percent);
    }
    targetLocked = true;
    Controller1.Screen.print("target locked!");
  }
}

void shoot(int speed) {
  Flywheel.setVelocity(speed, percent);
  Flywheel.spin(forward);
  vex::task::sleep(2000);
  Finger.setVelocity(50, percent);
  Finger.spin(forward);
  vex::task::sleep(3000);
  Finger.stop();
  Flywheel.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  while (rollingOut) {

    if (Controller1.ButtonA.pressing()) {
       targetLocked = false;
       targeting(Vision__GOAL);
      shoot(65);
    }
    // use gps to grab distance from goal
    // angle robot so its facing the goal
    // move to the optimal distance
    // shoot
  }
}
