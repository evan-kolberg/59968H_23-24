#include "main.h"
#include "okapi/api.hpp"
#include "MiniPID.h"

using namespace pros;

Controller master(E_CONTROLLER_MASTER);

ADIDigitalOut solenoid('H');
ADIDigitalIn limitSwitchTriball('A');
ADIDigitalIn limitSwitchCata('B');

Imu inertial(8);

double destination = 0;

Motor leftmotor1(1, E_MOTOR_GEARSET_06, true);
Motor leftmotor2(2, E_MOTOR_GEARSET_06, true);
Motor leftmotor3(3, E_MOTOR_GEARSET_06, true);

Motor rightmotor1(11, E_MOTOR_GEARSET_06);
Motor rightmotor2(12, E_MOTOR_GEARSET_06);
Motor rightmotor3(13, E_MOTOR_GEARSET_06);

MotorGroup leftdrive({leftmotor1, leftmotor2, leftmotor3});
MotorGroup rightdrive({rightmotor1, rightmotor2, rightmotor3});

Motor cata(6, E_MOTOR_GEARSET_36);
Motor intake(7, E_MOTOR_GEARSET_18);

void checkController()
{
	bool solstate = false;

	while (true)
	{
		int R = master.get_digital(E_CONTROLLER_DIGITAL_R1) - master.get_digital(E_CONTROLLER_DIGITAL_R2);

		intake.move_velocity(R * 200);

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B))
		{
			solenoid.set_value(solstate = !solstate);
			lcd::print(0, solstate ? "true: solenoid activated" : "false: air released");
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
				cata.move_velocity(100);
				pros::delay(250);
			}
		}
		else
		{
			cata.move_velocity(100);
		}

		pros::delay(20);
	}
}

void initialize()
{
	lcd::initialize();
	lcd::set_background_color(LV_COLOR_SILVER);

	lcd::print(0, "");
}

void disabled() {}

void competition_initialize() {}

struct turnPIDValues
{
	double p;
	double i;
	double d;
};

void turn_to_abs(double destination, turnPIDValues pidValues)
{
	MiniPID turnPID = MiniPID(pidValues.p, pidValues.i, pidValues.d);

	double sensor_value = inertial.get_rotation();

	while (true)
	{
		sensor_value = inertial.get_rotation();
		master.print(0, 0, "Heading: %.2f", sensor_value);
		master.clear();
		double output = turnPID.getOutput(sensor_value, destination);
		leftdrive.move(output);
		rightdrive.move(-output);

		pros::delay(20);
	}
}

void autonomous()
{
	using namespace okapi;

	double destination = 0;

	leftdrive.set_brake_modes(E_MOTOR_BRAKE_BRAKE);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_BRAKE);
	intake.set_brake_mode(E_MOTOR_BRAKE_HOLD);

	auto chassis = ChassisControllerBuilder()
					   .withMotors({1, 2, 3}, {11, 12, 13}) // 1, 2, 3 are alr reversed b/c of motor constructors
															// 5:3 gear ratio (36 input teeth, 60 output), 3.25 inch diameter wheels, 14 inch wheeltrack, blue motors
					   .withDimensions({AbstractMotor::gearset::blue, (60. / 36.)}, {{3.25_in, 14_in}, imev5BlueTPR})
					   // {P, I, D}
					   .withGains(
						   {0.0007, 0, 0.000}, // distance control
						   {0, 0, 0}		   // turn control ~ don't use this if using the inertial sensor
						   )
					   .build();

	chassis->setMaxVelocity(600);

	chassis->moveDistance(0.5_m);

	turnPIDValues pidValues = {0.86, 0, 0.6284};

	turn_to_abs(90, pidValues);
}

void opcontrol()
{
	leftdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightdrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	cata.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

	Task checkControllerThread(checkController);
	Task cataProcessThread(cataProcess);

	lcd::register_btn0_cb([] {});
	lcd::register_btn1_cb([] {});
	lcd::register_btn2_cb([] {});

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

