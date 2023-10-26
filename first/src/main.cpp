#include "main.h"
	
pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut ledA('A');
pros::ADIDigitalOut ledB('B');
pros::ADIDigitalOut ledC('C');

pros::ADIDigitalOut solenoid('H');

pros::Motor leftMotor1(11);
pros::Motor leftMotor2(12);
pros::Motor leftMotor3(13);

pros::Motor rightMotor1(1);
pros::Motor rightMotor2(2);
pros::Motor rightMotor3(3);

pros::MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
pros::MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});


bool leftLLEMUpressed;
bool centerLLEMUpressed;
bool rightLLEMUpressed;


void checkPneumatics() {
	while (true) {
		if (master.get_digital(DIGITAL_B) || leftLLEMUpressed) {
			solenoid.set_value(0);
			pros::lcd::print(1, "Pneumatics Triggered");
			pros::delay(1000);
		} else {
			solenoid.set_value(1);
		}

		pros::delay(1000);
	}
}


void initialize() {
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb([]{});
	pros::lcd::register_btn1_cb([]{});
	pros::lcd::register_btn2_cb([]{});

}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	while (true) {
		pros::lcd::print(7, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// based on the read_button() function, set the LLEMU variables above


		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick*(-1));

		pros::Task checkPneumaticsThread(checkPneumatics);

		pros::delay(20);
	}
}

