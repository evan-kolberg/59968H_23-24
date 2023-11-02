#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut solenoid('H');

pros::Motor leftMotor1(11, pros::E_MOTOR_GEARSET_06, true);
pros::Motor leftMotor2(12, pros::E_MOTOR_GEARSET_06, true);
pros::Motor leftMotor3(13, pros::E_MOTOR_GEARSET_06, true);

pros::Motor rightMotor1(1, pros::E_MOTOR_GEARSET_06);
pros::Motor rightMotor2(2, pros::E_MOTOR_GEARSET_06);
pros::Motor rightMotor3(3, pros::E_MOTOR_GEARSET_06);

pros::MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
pros::MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});

pros::Motor cata(6, pros::E_MOTOR_GEARSET_36);
pros::Motor intake(7, pros::E_MOTOR_GEARSET_06);

bool solenoidState = false;

void checkController() {
	while (true) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // intake in
			intake.move_velocity(600);
			pros::lcd::print(2, "intake in");
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // intake out
			intake.move_velocity(-600);
			pros::lcd::print(2, "intake out");
		} else {
			pros::lcd::clear_line(2);
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) { // cata
			cata.move_velocity(100);
			pros::lcd::print(1, "cata moving");
		} else {
			cata.move_velocity(0);
			pros::lcd::clear_line(1);
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			if (solenoidState) {
				solenoid.set_value(false);
				solenoidState = false;
				pros::lcd::print(0, "air released");
			} else {
				solenoid.set_value(true);
				solenoidState = true;
				pros::lcd::print(0, "solenoid activated");
			}
			
		}

		pros::delay(10);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_background_color(LV_COLOR_AQUA);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	leftDrive.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	rightDrive.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

	pros::Task checkControllerThread(checkController);

	pros::lcd::register_btn0_cb([]{}); // put a callback in here
	pros::lcd::register_btn1_cb([]{});
	pros::lcd::register_btn2_cb([]{});

	while (true) {
		pros::lcd::print(7, "LLEMU: %d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick);

		pros::delay(20);
	}
}

