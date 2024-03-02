#include "autons.hpp"
#include "main.h"

// These are out of 127
const int DRIVE_SPEED = 125;
const int TURN_SPEED = 100;
const int SWING_SPEED = 100;

void default_constants()
{
  chassis.pid_heading_constants_set(3, 0, 20);
  chassis.pid_drive_constants_set(10, 0, 80);
  chassis.pid_turn_constants_set(3, 0, 20);
  chassis.pid_swing_constants_set(5, 0, 30);

  chassis.pid_turn_exit_condition_set(15_ms, 1_deg, 25_ms, 3_deg, 250_ms, 250_ms);
  chassis.pid_swing_exit_condition_set(15_ms, 1_deg, 25_ms, 3_deg, 250_ms, 250_ms);
  chassis.pid_drive_exit_condition_set(15_ms, 1_in, 25_ms, 2_in, 250_ms, 250_ms);

  chassis.slew_drive_constants_set(5_in, 105);
}

void far_side_6_ball()
{
  intake.move_velocity(200);
  pros::delay(675);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_swing_set(ez::LEFT_SWING, -45_deg, SWING_SPEED, 30); chassis.pid_wait();
  back_left_wing.set(true);
  chassis.pid_drive_set(-10.5_in, DRIVE_SPEED, true); chassis.pid_wait();
  back_left_wing.set(false);
  chassis.pid_swing_set(ez::LEFT_SWING, -90_deg, SWING_SPEED, 35); chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_swing_set(ez::LEFT_SWING, 65_deg, SWING_SPEED, 15); chassis.pid_wait();
  chassis.pid_drive_set(17_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_swing_set(ez::LEFT_SWING, 140_deg, SWING_SPEED, 5); chassis.pid_wait();
  intake.move_velocity(-200);
  pros::delay(600);
  chassis.pid_turn_set(0_deg, TURN_SPEED); chassis.pid_wait();
  intake.move_velocity(200);
  chassis.pid_drive_set(20_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_turn_set(145_deg, TURN_SPEED); chassis.pid_wait();
  intake.move_velocity(-200);
  pros::delay(600);
  chassis.pid_turn_set(90_deg, TURN_SPEED); chassis.pid_wait();
  intake.move_velocity(200);
  chassis.pid_swing_set(ez::RIGHT_SWING, 63_deg, SWING_SPEED, 55); chassis.pid_wait();
  chassis.pid_turn_set(30_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(4_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_turn_set(180_deg, TURN_SPEED); chassis.pid_wait();
  front_left_wing.set(true);
  front_right_wing.set(true);
  intake.move_velocity(-200);
  chassis.pid_drive_set(35_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_drive_set(-30_in, DRIVE_SPEED, true); chassis.pid_wait();
  front_left_wing.set(false);
  front_right_wing.set(false);
}

void far_side_4_ball() {
    front_right_wing.set(true);
    pros::delay(250);
    front_right_wing.set(false);
    chassis.pid_turn_set(-18_deg, TURN_SPEED); chassis.pid_wait();
    intake.move_velocity(200);
    chassis.pid_drive_set(52_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_drive_set(-50_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_turn_set(-90_deg, TURN_SPEED); chassis.pid_wait();
    back_left_wing.set(true);
    chassis.pid_swing_set(ez::LEFT_SWING, -180_deg, SWING_SPEED, 35); chassis.pid_wait();
    back_left_wing.set(false);
    chassis.pid_drive_set(-8_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 17); chassis.pid_wait();
    chassis.pid_drive_set(38_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_turn_set(90_deg, TURN_SPEED); chassis.pid_wait();
    intake.move_velocity(-200);
    chassis.pid_drive_set(11_in, DRIVE_SPEED, true); chassis.pid_wait();
    intake.move_velocity(0);
    chassis.pid_drive_set(-10_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_turn_set(-117_deg, TURN_SPEED); chassis.pid_wait();
    intake.move_velocity(200);
    chassis.pid_drive_set(23_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_drive_set(-23_in, DRIVE_SPEED, true); chassis.pid_wait();
    chassis.pid_turn_set(90_deg, TURN_SPEED); chassis.pid_wait();
    front_left_wing.set(true);
    front_right_wing.set(true);
    intake.move_velocity(-200);
    chassis.pid_drive_set(15_in, DRIVE_SPEED, true); chassis.pid_wait();
    intake.move_velocity(0);
    chassis.pid_drive_set(-10_in, DRIVE_SPEED, true); chassis.pid_wait();
    front_left_wing.set(false);
    front_right_wing.set(false);
}

void close_side()
{

}

void prog_skills()
{
  pros::Task cata_process_thread(cata_process);
  pros::delay(50000);
}



















///
// Drive Example
///
void drive_example()
{
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example()
{
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn()
{
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed()
{
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 30
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(30); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 30 speed
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 30
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(30); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 30 speed
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example()
{
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements()
{
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts)
{
  for (int i = 0; i < attempts - 1; i++)
  {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered)
    {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(800);
    }
    // If robot successfully drove back, return
    else
    {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees.
// If interfered, robot will drive forward and then attempt to drive backwards.
void interfered_example()
{
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered)
  {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}
