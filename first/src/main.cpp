#include "main.h"
#include "okapi/api.hpp"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut solenoid('H');

Motor leftMotor1(11, E_MOTOR_GEARSET_06, true);
Motor leftMotor2(12, E_MOTOR_GEARSET_06, true);
Motor leftMotor3(13, E_MOTOR_GEARSET_06, true);

Motor rightMotor1(1, E_MOTOR_GEARSET_06);
Motor rightMotor2(2, E_MOTOR_GEARSET_06);
Motor rightMotor3(3, E_MOTOR_GEARSET_06);

MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});

Motor cata(6, E_MOTOR_GEARSET_36);
Motor intake(7, E_MOTOR_GEARSET_06);

bool solenoidState = false;

void checkController() {
	while (true) {
		if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) { // intake in
			intake.move_velocity(600);
			lcd::print(2, "intake in");
		} else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) { // intake out
			intake.move_velocity(-600);
			lcd::print(2, "intake out");
		} else {
			lcd::clear_line(2);
		}

		if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) { // cata
			cata.move_velocity(100);
			lcd::print(1, "cata moving");
		} else {
			cata.move_velocity(0);
			lcd::clear_line(1);
		}

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
			if (solenoidState) {
				solenoid.set_value(false);
				solenoidState = false;
				lcd::print(0, "air released");
			} else {
				solenoid.set_value(true);
				solenoidState = true;
				lcd::print(0, "solenoid activated");
			}
			
		}

		delay(10);
	}
}

void initialize() {
	lcd::initialize();
	lcd::set_background_color(LV_COLOR_AQUA);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    using namespace okapi;

    auto chassis = ChassisControllerBuilder()
        .withMotors({-11, -12, -13}, {1, 2, 3})
        .withDimensions(AbstractMotor::gearset::blue, {{(4_in * 5) / 3, 10_in, 16_in, 140_in}, imev5BlueTPR})
        .build();

    auto profileController = AsyncMotionProfileControllerBuilder()
        .withLimits({1.0, 2.0, 10.0}) // velocity, acceleration, jerk
        .withOutput(chassis)
        .buildMotionProfileController();

    // x, y, heading theta
    profileController->generatePath({
        {0_ft, 0_ft, 0_deg},
        {3_ft, 0_ft, 0_deg},
        {3_ft, 3_ft, 90_deg}
    }, "first"); // def path name

	profileController->generatePath({
        {0_ft, 0_ft, 0_deg},
        {3_ft, 0_ft, 0_deg},
        {3_ft, 3_ft, 90_deg}
    }, "second"); // def path name


    profileController->setTarget("first");
	profileController->setTarget("second");

    profileController->waitUntilSettled();

}



void opcontrol() {
	leftDrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightDrive.set_brake_modes(E_MOTOR_BRAKE_COAST);

	Task checkControllerThread(checkController);

	lcd::register_btn0_cb([]{}); // put a callback in here
	lcd::register_btn1_cb([]{});
	lcd::register_btn2_cb([]{});

	while (true) {
		lcd::print(7, "LLEMU: %d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick);

		delay(20);
	}
}

