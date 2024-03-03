#include "autons.hpp"
#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/rtos.hpp"

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

void prog_skills()
{
  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 30); chassis.pid_wait();
  chassis.pid_drive_set(-10.5_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_swing_set(ez::RIGHT_SWING, 90_deg, SWING_SPEED, 35); chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_swing_set(ez::RIGHT_SWING, -18_deg, SWING_SPEED, 0); chassis.pid_wait();
  chassis.pid_drive_set(-6_in, DRIVE_SPEED, false); chassis.pid_wait();
  back_right_wing.set(true);
  // pros::Task cata_process_thread(cata_process);
  cata.move_velocity(40);
  cata_2.move_velocity(-40);
  pros::delay(23000);
  cata.move_velocity(0);
  cata_2.move_velocity(0);
  back_right_wing.set(false);
  chassis.pid_turn_set(30_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_turn_set(0_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(69_in, DRIVE_SPEED, true); chassis.pid_wait();
  front_left_wing.set(true);
  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 30); chassis.pid_wait();
  chassis.pid_drive_set(10.5_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_swing_set(ez::RIGHT_SWING, -90_deg, SWING_SPEED, 35); chassis.pid_wait();
  front_left_wing.set(false);
  chassis.pid_drive_set(10_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_drive_set(-9_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_turn_set(-165_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(42_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_turn_set(-90_deg, TURN_SPEED); chassis.pid_wait();
  front_left_wing.set(true);
  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 23); chassis.pid_wait();
  chassis.pid_drive_set(18_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_drive_set(-15_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_drive_set(16_in, DRIVE_SPEED, true); chassis.pid_wait();
  front_left_wing.set(false);
  chassis.pid_swing_set(ez::LEFT_SWING, -90_deg, SWING_SPEED, 45); chassis.pid_wait();
  chassis.pid_drive_set(50_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_turn_set(0_deg, TURN_SPEED); chassis.pid_wait();
  front_left_wing.set(true);
  front_right_wing.set(true);
  chassis.pid_turn_set(65_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 65); chassis.pid_wait();
  chassis.pid_drive_set(20_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_drive_set(-25_in, DRIVE_SPEED, true); chassis.pid_wait();
  front_left_wing.set(false);
  front_right_wing.set(false);
  chassis.pid_turn_set(115_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(-55_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_turn_set(180_deg, TURN_SPEED); chassis.pid_wait();
  back_left_wing.set(true);
  chassis.pid_swing_set(ez::RIGHT_SWING, 225_deg, SWING_SPEED, 30); chassis.pid_wait();
  back_left_wing.set(false);
  chassis.pid_drive_set(-10.5_in, DRIVE_SPEED, true); chassis.pid_wait();
  chassis.pid_swing_set(ez::RIGHT_SWING, 270_deg, SWING_SPEED, 35); chassis.pid_wait();
  chassis.pid_turn_set(270_deg, TURN_SPEED); chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED, false); chassis.pid_wait();
  chassis.pid_drive_set(10_in, DRIVE_SPEED, false); chassis.pid_wait();
}

