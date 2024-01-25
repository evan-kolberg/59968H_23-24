#include "main.h"
#include "okapi/api.hpp"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut solenoid('A');

Imu inertial(8);

double destination = 0;


Motor rightmotor1(8, E_MOTOR_GEARSET_36, true);
Motor rightmotor2(9, E_MOTOR_GEARSET_36);

Motor leftmotor1(15, E_MOTOR_GEARSET_36);
Motor leftmotor2(10, E_MOTOR_GEARSET_36);


MotorGroup leftdrive({leftmotor1, leftmotor2});
MotorGroup rightdrive({rightmotor1, rightmotor2});

Motor cata(20, E_MOTOR_GEARSET_36);
Motor cata2(16, E_MOTOR_GEARSET_36);

Motor intake(7, E_MOTOR_GEARSET_18);

void checkController() {
    bool solstate = false;

    while (true) {
        int R = master.get_digital(E_CONTROLLER_DIGITAL_R1) - master.get_digital(E_CONTROLLER_DIGITAL_R2);

        intake.move_velocity(R * 200);
        cata.move_velocity(master.get_digital(E_CONTROLLER_DIGITAL_L1) * -100);
		cata2.move_velocity(master.get_digital(E_CONTROLLER_DIGITAL_L1) * 100);


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

		int forwardBackward = master.get_analog(ANALOG_LEFT_Y); // Forward and backward control
		int leftRight = master.get_analog(ANALOG_RIGHT_X); // Left and right control

		// Arcade control calculations
		int leftMotorSpeed = forwardBackward - leftRight;
		int rightMotorSpeed = forwardBackward + leftRight;

		leftdrive.move(leftMotorSpeed);
		rightdrive.move(rightMotorSpeed);

		delay(20);

	}
}

