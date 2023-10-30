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


void actuatePneu() {
	solenoid.set_value(true);
	pros::lcd::print(0, "Actuated");
}

void retractPneu() {
	solenoid.set_value(false);
	pros::lcd::print(0, "Retracted");
}

void checkController() {
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
		actuatePneu();
	}
	pros::delay(10);

	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		retractPneu();
	}
	pros::delay(10);
}

void initialize() {
	pros::lcd::initialize();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	pros::Task checkControllerThread(checkController);

	pros::lcd::register_btn0_cb(actuatePneu);
	pros::lcd::register_btn1_cb(retractPneu);
	pros::lcd::register_btn2_cb([]{});

	while (true) {
		pros::lcd::print(7, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick*(-1));

		pros::delay(20);
	}
}

