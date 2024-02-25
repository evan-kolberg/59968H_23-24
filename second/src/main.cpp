#include "main.h"

ez::Drive chassis({-11, -12, -13}, {1, 2, 3}, 8, 3.25, 600, (60. / 36.));

pros::Motor cata(20, pros::E_MOTOR_GEARSET_36);
pros::Motor intake(7, pros::E_MOTOR_GEARSET_18);

ez::Piston front_left_wing('A', false);
ez::Piston front_right_wing('B', false);

ez::Piston back_left_wing('C', false);
//ez::Piston back_right_wing('D', false);

ez::Piston elevation_1('E', false);
ez::Piston elevation_2('F', false);

pros::ADIDigitalIn limit_switch_cata('D');
pros::Distance dist_sensor(19);

void cata_process() {
  while (true) {
    if (limit_switch_cata.get_value()) {
      cata.move_velocity(0);

      if (dist_sensor.get() < 5) {
        cata.move_velocity(-100);
        pros::delay(250);
      }

    } else {
      cata.move_velocity(-100);
    }

    pros::delay(20);
  }
}

void initialize() {
  ez::ez_template_print();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  //chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the
  // controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.1); // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0); // Defaults for curve. If using tank, only the first parameter is
             // used. (Comment this line out if you have an SD card!)
  default_constants(); // Set the drive to your own constants from autons.cpp!

  ez::as::auton_selector.autons_add({
      Auton("\nFar Side 6-Ball", far_side_6_ball),
      Auton("\nFar Side 4-Ball", far_side_4_ball),
      Auton("\nClose Side", close_side),
      Auton("\nProgramming Skills", prog_skills),
      Auton("\nExample Drive\n\nDrive forward and come back.", drive_example),
      Auton("\nExample Turn\n\nTurn 3 times.", turn_example),
      Auton("\nDrive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
      Auton("\nDrive and Turn\n\nSlow down during drive.", wait_until_change_speed),
      Auton("\nSwing Example\n\nSwing in an 'S' curve", swing_example),
      Auton("\nCombine all 3 movements", combining_movements),
      Auton("\nInterference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
  });

  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
}


void disabled() {

}

void competition_initialize() {
  
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();               // Resets PID targets to 0
  chassis.drive_imu_reset();                 // Reset gyro position to 0
  chassis.drive_sensor_reset();              // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps
                                             // autonomous consistency

  ez::as::auton_selector.selected_auton_call(); // Calls selected auton from autonomous selector
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  //pros::Task cata_process_thread(cata_process);

  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  while (true) {

    if (!pros::competition::is_connected()) {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      /*
      if (master.get_digital_new_press(DIGITAL_))
        chassis.pid_tuner_toggle();
      */
      if (master.get_digital_new_press(DIGITAL_A))
        autonomous();
      chassis.drive_brake_set(MOTOR_BRAKE_COAST); // back to normal after auton

      chassis.pid_tuner_iterate();
    }

    chassis.opcontrol_tank();

    int R = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) -
            master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    intake.move_velocity(R * 200);

    front_left_wing.button_toggle(master.get_digital(DIGITAL_L1));
    front_right_wing.button_toggle(master.get_digital(DIGITAL_L1));

    back_left_wing.button_toggle(master.get_digital(DIGITAL_L2));
    //back_right_wing.button_toggle(master.get_digital(DIGITAL_L2));

    elevation_1.button_toggle(master.get_digital(DIGITAL_X));
    elevation_2.button_toggle(master.get_digital(DIGITAL_X));

    pros::lcd::print(7, "Distance %d", dist_sensor.get());

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!
                                       // Keep this ez::util::DELAY_TIME
  }
}
