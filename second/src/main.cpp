#include "main.h"

// Chassis constructor
ez::Drive chassis({-11, -12, -13}, {1, 2, 3}, 8, 3.25, 600, (60. / 36.));

pros::Motor cata(20, pros::E_MOTOR_GEARSET_36);
pros::Motor intake(7, pros::E_MOTOR_GEARSET_18);

ez::Piston front_left_wing('A', false);  // dual action, 1 piston
ez::Piston front_right_wing('B', false); // dual action, 1 piston

ez::Piston back_left_wing('C', false);  // single action, 1 piston
ez::Piston back_right_wing('D', false); // single action, 1 piston

ez::Piston elevation_1('E', false); // dual action, 2 pistons
ez::Piston elevation_2('F', false); // dual action, 2 pistons

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.1); // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0);    // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants();                          // Set the drive to your own constants from autons.cpp!

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      Auton("\nFar Side", far_side_4_ball),
      Auton("\nClose Side", close_side),
      Auton("\nSkills", skills),
      Auton("\nExample Drive\n\nDrive forward and come back.", drive_example),
      Auton("\nExample Turn\n\nTurn 3 times.", turn_example),
      Auton("\nDrive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
      Auton("\nDrive and Turn\n\nSlow down during drive.", wait_until_change_speed),
      Auton("\nSwing Example\n\nSwing in an 'S' curve", swing_example),
      Auton("\nCombine all 3 movements", combining_movements),
      Auton("\nInterference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
  // . . .
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
void autonomous()
{
  chassis.pid_targets_reset();               // Resets PID targets to 0
  chassis.drive_imu_reset();                 // Reset gyro position to 0
  chassis.drive_sensor_reset();              // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

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
void opcontrol()
{
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  bool cata_state = false;

  while (true)
  {

    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected())
    {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      /*
      if (master.get_digital_new_press(DIGITAL_))
        chassis.pid_tuner_toggle();
      */
      // Trigger the selected autonomous routine
      if (master.get_digital_new_press(DIGITAL_A))
        autonomous();
      chassis.drive_brake_set(MOTOR_BRAKE_COAST); // back to normal after auton

      chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
    }

    chassis.opcontrol_tank(); // Tank control

    int R = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) - master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    intake.move_velocity(R * 200);

    if (master.get_digital_new_press(DIGITAL_B))
    {
      cata_state = !cata_state;
      cata.move_velocity(cata_state ? 100 : 0);
    }

    front_left_wing.button_toggle(master.get_digital(DIGITAL_L2));
    front_right_wing.button_toggle(master.get_digital(DIGITAL_L2));

    back_left_wing.button_toggle(master.get_digital(DIGITAL_L1));
    back_right_wing.button_toggle(master.get_digital(DIGITAL_L1));

    elevation_1.button_toggle(master.get_digital(DIGITAL_X));
    elevation_2.button_toggle(master.get_digital(DIGITAL_X));

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
