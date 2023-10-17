#include "main.h"

pros::ADIDigitalOut ledA('A');
pros::ADIDigitalOut ledB('B');
pros::ADIDigitalOut ledC('C');
pros::ADIDigitalOut ledH('H');

pros::Motor leftMotor1(11);
pros::Motor leftMotor2(12);
pros::Motor leftMotor3(13);

pros::Motor rightMotor1(1);
pros::Motor rightMotor2(2);
pros::Motor rightMotor3(3);

pros::MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
pros::MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});

bool lowBatteryActionRequired = false;
bool pneumaticsLow = false;



void on_left_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
	} else {
	}
}

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
	} else {
	}
}

void on_right_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
	} else {
	}
}

void blindedByTheLights() {
    while (true) {
        if (lowBatteryActionRequired) {
            ledA.set_value(0); /* On */
            ledB.set_value(0);
            ledC.set_value(0);
            pros::delay(25);
            ledA.set_value(1); /* Off */
            ledB.set_value(1);
            ledC.set_value(1);
            pros::delay(25);
        }
        pros::delay(100); // Adjust this delay as needed
    }
}

void checkBattery(int percent) {
    while (true) {
        int batteryCapacity = pros::battery::get_capacity();
        int batteryPercentage = (batteryCapacity * 100) / 1100; // divide by mAh

        if (batteryPercentage <= percent) {
            lowBatteryActionRequired = true;
        } else {
            lowBatteryActionRequired = false;
        }

        pros::delay(5000);
    }
}





/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);

	/* 0 is On and 1 is Off */
	ledH.set_value(0);
	ledA.set_value(1);
	ledB.set_value(1);
	ledC.set_value(1);

	pros::Task redLEDthread(blindedByTheLights);
	pros::Task batteryCheckThread(checkBattery, 5);



}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {}

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick);

		pros::delay(20);
	}
}
