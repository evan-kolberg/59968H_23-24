#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut solenoid1('A');
pros::ADIDigitalOut solenoid2('H');

bool solenoidState = false;

void checkController() {
	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			if (solenoidState) {
				solenoid1.set_value(false);
				solenoid2.set_value(false);
				solenoidState = false;
				pros::lcd::print(0, "false: air released"); // brass piston input on A, return on B
				// when the program is off, A should be closed and B should be open
				// reversing the A and B valvues will cause the pistons to be activated
				// when the values are set to false AKA off
			} else {
				solenoid1.set_value(true);
				solenoid2.set_value(true);
				solenoidState = true;
				pros::lcd::print(0, "true: solenoids activated");
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
	pros::Task checkControllerThread(checkController);

	pros::lcd::register_btn0_cb([]{});
	pros::lcd::register_btn1_cb([]{});
	pros::lcd::register_btn2_cb([]{});

	while (true) {
		pros::lcd::print(7, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		pros::delay(20);
	}
}

