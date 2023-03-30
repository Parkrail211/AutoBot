#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FlywheelMotorA = motor(PORT8, ratio6_1, true);
motor FlywheelMotorB = motor(PORT17, ratio6_1, false);
motor_group Flywheel = motor_group(FlywheelMotorA, FlywheelMotorB);
motor MotorsRMotorA = motor(PORT20, ratio18_1, false);
motor MotorsRMotorB = motor(PORT16, ratio18_1, false);
motor_group MotorsR = motor_group(MotorsRMotorA, MotorsRMotorB);
motor MotorsLMotorA = motor(PORT19, ratio18_1, false);
motor MotorsLMotorB = motor(PORT18, ratio18_1, false);
motor_group MotorsL = motor_group(MotorsLMotorA, MotorsLMotorB);
motor Finger = motor(PORT11, ratio18_1, false);
inertial Inertial = inertial(PORT14);
/*vex-vision-config:begin*/
signature Vision__GOAL = signature (1, 3181, 3493, 3337, -1, 287, 143, 4.6, 0);
vision Vision = vision (PORT13, 79, Vision__GOAL);
/*vex-vision-config:end*/
controller Controller1 = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}