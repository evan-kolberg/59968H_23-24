#include "main.h"
#include "okapi/api.hpp"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut solenoid('H');

Motor leftMotor1(1, E_MOTOR_GEARSET_06, true);
Motor leftMotor2(2, E_MOTOR_GEARSET_06, true);
Motor leftMotor3(3, E_MOTOR_GEARSET_06, true);

Motor rightMotor1(11, E_MOTOR_GEARSET_06);
Motor rightMotor2(12, E_MOTOR_GEARSET_06);
Motor rightMotor3(13, E_MOTOR_GEARSET_06);

MotorGroup leftdrive({leftMotor1, leftMotor2, leftMotor3});
MotorGroup rightdrive({rightMotor1, rightMotor2, rightMotor3});

Motor cata(6, E_MOTOR_GEARSET_36);
Motor intake(7, E_MOTOR_GEARSET_06);

void checkController() {
    bool solstate = false;

    while (true) {
        int R = master.get_digital(E_CONTROLLER_DIGITAL_R2) - master.get_digital(E_CONTROLLER_DIGITAL_R1);
        int L = master.get_digital(E_CONTROLLER_DIGITAL_L1);

        intake.move_velocity(R * 600);
        cata.move_velocity(L * 100);

        if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            solenoid.set_value(solstate = !solstate);
            lcd::print(0, solstate ? "true: solenoid activated" : "false: air released");
        }

        delay(10);
    }
}

void initialize() {
	lcd::initialize();
	lcd::set_background_color(LV_COLOR_SILVER);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    using namespace okapi;

	leftdrive.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	cata.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(E_MOTOR_BRAKE_HOLD);

    auto chassis = ChassisControllerBuilder()
        .withMotors({-1, -2, -3}, {11, 12, 13})
    	.withDimensions({AbstractMotor::gearset::blue, (5 / 3)}, {{3.25_in, 12.5_in}, imev5BlueTPR})
        .build();

    auto profileController = AsyncMotionProfileControllerBuilder()
        .withLimits({1.0, 2.0, 10.0})
        .withOutput(chassis)
        .buildMotionProfileController();

    profileController->generatePath({
        {0_ft, 0_ft, 0_deg},
        {3_ft, 0_ft, 0_deg},
    }, "first");

    profileController->setTarget("first");

	profileController->waitUntilSettled();
}



void opcontrol() {
	leftdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	cata.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

	Task checkControllerThread(checkController);

	lcd::register_btn0_cb([]{});
	lcd::register_btn1_cb([]{});
	lcd::register_btn2_cb([]{});

	while (true) {
		lcd::print(7, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftdrive.move(leftStick);
		rightdrive.move(rightStick);

		delay(20);
	}
}

