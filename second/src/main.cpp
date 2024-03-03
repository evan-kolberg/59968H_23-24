#include "main.h"
#include "pros/misc.h"

ez::Drive chassis(
{-11, -12, -13},
{1, 2, 3},
8,
3.25,
600,
(60. / 36.));

pros::Motor cata(20, pros::E_MOTOR_GEARSET_36);
pros::Motor cata_2(18, pros::E_MOTOR_GEARSET_36);
pros::Motor intake(7, pros::E_MOTOR_GEARSET_18);

ez::Piston front_left_wing('A', false);
ez::Piston front_right_wing('B', false);

ez::Piston back_left_wing('C', false);
ez::Piston back_right_wing('D', false);

ez::Piston elevation_1('H', false);
ez::Piston elevation_2('F', false);

pros::ADIDigitalIn limit_switch_cata('G');
pros::Distance dist_sensor(19);

void cata_process() {
  while (true)
  {
    if (limit_switch_cata.get_value())
    {
      cata.move_velocity(0);
      cata_2.move_velocity(0);
      if (dist_sensor.get() < 5)
      {
        cata.move_velocity(100);
        cata_2.move_velocity(-100);
        pros::delay(250);
      }
    }
    else
    {
      cata.move_velocity(100);
      cata_2.move_velocity(-100);
    }
    pros::delay(ez::util::DELAY_TIME);
  }
}

void initialize() {

  chassis.opcontrol_drive_activebrake_set(0.1);
  chassis.opcontrol_curve_buttons_toggle(false); 
  chassis.opcontrol_curve_default_set(0, 0); 

  default_constants();

  ez::as::auton_selector.autons_add({
      Auton("\nFar Side 6-Ball", far_side_6_ball),
      Auton("\nFar Side 4-Ball", far_side_4_ball),
      Auton("\nProgramming Skills", prog_skills),
  });

  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
}


void disabled() {

}

void competition_initialize() {

}


void autonomous() {
  chassis.pid_targets_reset();
  chassis.drive_imu_reset();
  chassis.drive_sensor_reset();
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);

  ez::as::auton_selector.selected_auton_call();
}


void opcontrol() {

  // pros::Task cata_process_thread(cata_process);


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
      chassis.drive_brake_set(MOTOR_BRAKE_COAST);

      chassis.pid_tuner_iterate();
    }

    chassis.opcontrol_tank();

    int R = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) -
            master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    intake.move_velocity(R * 200);

    front_left_wing.button_toggle(master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_LEFT));
    front_right_wing.button_toggle(master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_RIGHT));

    back_left_wing.button_toggle(master.get_digital(DIGITAL_L2) || master.get_digital(DIGITAL_DOWN));
    back_right_wing.button_toggle(master.get_digital(DIGITAL_L2) || master.get_digital(DIGITAL_UP));

    elevation_1.button_toggle(master.get_digital(DIGITAL_X));
    elevation_2.button_toggle(master.get_digital(DIGITAL_X));

    cata.move_velocity(master.get_digital(DIGITAL_B) * 40);
    cata_2.move_velocity(master.get_digital(DIGITAL_B) * -40);

    pros::delay(ez::util::DELAY_TIME);
  }
}
