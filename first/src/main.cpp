#include "main.h"
#include <string>
#include "okapi/api.hpp"
#include "MiniPID.h"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut wings1('H');
ADIDigitalOut wings2('E');
ADIDigitalOut elevation1('G');
ADIDigitalOut elevation2('F');

ADIDigitalIn limitSwitchTriball('A');
ADIDigitalIn limitSwitchCata('C');

Imu inertial(8);

Motor leftmotor1(11, E_MOTOR_GEARSET_06, true);
Motor leftmotor2(12, E_MOTOR_GEARSET_06, true);
Motor leftmotor3(13, E_MOTOR_GEARSET_06, true);

Motor rightmotor1(1, E_MOTOR_GEARSET_06);
Motor rightmotor2(1, E_MOTOR_GEARSET_06);
Motor rightmotor3(3, E_MOTOR_GEARSET_06);

MotorGroup leftdrive({leftmotor1, leftmotor2, leftmotor3});
MotorGroup rightdrive({rightmotor1, rightmotor2, rightmotor3});

Motor cata(6, E_MOTOR_GEARSET_36);
Motor intake(7, E_MOTOR_GEARSET_18);

void checkController()
{
	bool wstate = false;
	bool elstate = false;

	while (true)
	{
		int R = master.get_digital(E_CONTROLLER_DIGITAL_R1) - master.get_digital(E_CONTROLLER_DIGITAL_R2);

		intake.move_velocity(R * 200);

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B))
		{
			bool new_wstate = (wstate = !wstate);
			wings1.set_value(new_wstate);
			wings2.set_value(new_wstate);
		}

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y))
		{
			bool new_elstate = (elstate = !elstate);
			elevation1.set_value(new_elstate);
			elevation2.set_value(new_elstate);
		}

		pros::delay(10);
	}
}

void cataProcess()
{
	while (true)
	{
		if (limitSwitchCata.get_value())
		{
			cata.move_velocity(0);
			if (limitSwitchTriball.get_value())
			{
				cata.move_velocity(-100);
				pros::delay(250);
			}
		}
		else
		{
			cata.move_velocity(-100);
		}

		pros::delay(20);
	}
}

std::string selectedAuton = "";

void initialize()
{
	lcd::initialize();
	lcd::set_background_color(LV_COLOR_RED);
}

void disabled() {}

void competition_initialize() {}

double destination = 0;

void turn_to_abs(void *)
{
	MiniPID turnPID = MiniPID(1.3, 0, 0.7);

	double sensor_value = inertial.get_rotation();

	while (true)
	{
		sensor_value = inertial.get_rotation();
		master.print(0, 0, "Heading: %.2f", sensor_value);
		master.clear();
		double output = turnPID.getOutput(sensor_value, destination);
		leftdrive.move(output);
		rightdrive.move(-output);

		delay(20);
	}
}

void autonomous()
{
	using namespace okapi;

	leftdrive.set_brake_modes(E_MOTOR_BRAKE_BRAKE);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_BRAKE);
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

	auto chassis = ChassisControllerBuilder()
					   .withMotors({1, 2, 3}, {11, 12, 13}) // 1, 2, 3 are alr reversed b/c of motor constructors
															// 5:3 gear ratio (36 input teeth, 60 output), 3.25 inch diameter wheels, 14 inch wheeltrack, blue motors
					   .withDimensions({AbstractMotor::gearset::blue, (60. / 36.)}, {{3.25_in, 14_in}, imev5BlueTPR})
					   // {P, I, D}
					   .withGains(
						   {0.002, 0, 0.000005}, // distance control
						   {0, 0, 0}			 // turn control ~ don't use this if using the inertial sensor
						   )
					   .build();

	// can't do a negative distance to reverse without fucking up the auton, everything stalls
	// use eztemplate and lemlib because c2c team uses that


	intake.move_velocity(200);
	chassis->moveDistance(1.1_m);
	intake.move_velocity(0);


}

void opcontrol()
{
	leftdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	cata.set_brake_mode(E_MOTOR_BRAKE_COAST);
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

	Task checkControllerThread(checkController);
	Task cataProcessThread(cataProcess);

	while (true)
	{
		lcd::print(7, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
				   (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
				   (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftdrive.move(leftStick);
		rightdrive.move(rightStick);

		pros::delay(20);
	}
}
