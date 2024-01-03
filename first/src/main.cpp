#include "main.h"
#include "okapi/api.hpp"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut solenoid('H');

Motor leftmotor1(1, E_MOTOR_GEARSET_06, true);
Motor leftmotor2(2, E_MOTOR_GEARSET_06, true);
Motor leftmotor3(3, E_MOTOR_GEARSET_06, true);

Motor rightmotor1(11, E_MOTOR_GEARSET_06);
Motor rightmotor2(12, E_MOTOR_GEARSET_06);
Motor rightmotor3(13, E_MOTOR_GEARSET_06);

MotorGroup leftdrive({leftmotor1, leftmotor2, leftmotor3});
MotorGroup rightdrive({rightmotor1, rightmotor2, rightmotor3});

Motor cata(6, E_MOTOR_GEARSET_36);
Motor intake(7, E_MOTOR_GEARSET_06);

void checkController() {
    bool solstate = false;

    while (true) {
        int R = master.get_digital(E_CONTROLLER_DIGITAL_R2) - master.get_digital(E_CONTROLLER_DIGITAL_R1);

        intake.move_velocity(R * 600);
        cata.move_velocity(master.get_digital(E_CONTROLLER_DIGITAL_L1) * -100);

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
		
	lcd::print(0, "");

}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    using namespace okapi;

	leftdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	cata.set_brake_mode(E_MOTOR_BRAKE_COAST);
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

    auto chassis = ChassisControllerBuilder()
        .withMotors({1, 2, 3}, {11, 12, 13}) // 1, 2, 3 are alr reversed b/c of motor constructors
		// 5:3 gear ratio (36 input teeth, 60 output), 3.25 inch diameter wheels, 13 inch wheeltrack, blue motors
    	.withDimensions({AbstractMotor::gearset::blue, (60 / 36)}, {{3.25_in, 13_in}, imev5BlueTPR})
		// {P, I, D}
		.withGains(
			{0.005, 0, 0.0005}, // distance control
			{0.001, 0, 0.0001}  // turn control
		)
        .build();

    auto profileController = AsyncMotionProfileControllerBuilder() // path planning controller
        .withLimits({.5, 2.0, 10.0}) // vel m/s, accel m/s^2, jerk m/s^3
        .withOutput(chassis)
        .buildMotionProfileController();

	// {x_unit, y_unit, headingθ_unit}
    profileController->generatePath({ // positive x is forward, positive y is to the right, robot starts at 0 on unit circle
		{0_m, 0_m, 0_deg},
		{1_m, 1_m, 0_deg},

    }, "auton");

	/* 
    profileController->setTarget("auton");
	profileController->waitUntilSettled();
	*/
	
	chassis->setMaxVelocity(100);
	chassis->moveDistance(1_m);
	chassis->turnAngle(90_deg*2);

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

