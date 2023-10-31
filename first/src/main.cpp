#include "main.h"
	
pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut ledA('A');
pros::ADIDigitalOut ledB('B');
pros::ADIDigitalOut ledC('C');

pros::ADIDigitalOut solenoid('H');

pros::Motor leftMotor1(11, pros::E_MOTOR_GEARSET_36); // blue
pros::Motor leftMotor2(12, pros::E_MOTOR_GEARSET_36);
pros::Motor leftMotor3(13, pros::E_MOTOR_GEARSET_36);

pros::Motor rightMotor1(1, pros::E_MOTOR_GEARSET_36);
pros::Motor rightMotor2(2, pros::E_MOTOR_GEARSET_36);
pros::Motor rightMotor3(3, pros::E_MOTOR_GEARSET_36);

pros::MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
pros::MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});

pros::Motor cata(6, pros::E_MOTOR_GEARSET_18); // red


void checkController() {
	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			solenoid.set_value(true);
			pros::lcd::print(0, "solenoid activated");
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			solenoid.set_value(false);
			pros::lcd::print(0, "air released");
		}
		
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			cata.move_velocity(200);
			pros::lcd::print(1, "cata moving");
		} else {
			cata.move_velocity(0);
			pros::lcd::clear_line(2);
		}

		pros::delay(10);
	}
}

void initialize() {
	pros::lcd::initialize();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
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

		leftDrive.move(leftStick*(-1));
		rightDrive.move(rightStick);

		pros::delay(20);
	}
}

