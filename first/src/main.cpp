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

MotorGroup leftDrive({leftMotor1, leftMotor2, leftMotor3});
MotorGroup rightDrive({rightMotor1, rightMotor2, rightMotor3});

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

	leftDrive.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	rightDrive.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	cata.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	intake.set_brake_mode(E_MOTOR_BRAKE_HOLD);

    auto chassis = ChassisControllerBuilder()
        .withMotors({-1, -2, -3}, {11, 12, 13})
        .withDimensions(AbstractMotor::gearset::blue, {{(4_in * 5) / 3, 10_in, 16_in, 140_in}, imev5BlueTPR})
        .build();

    auto profileController = AsyncMotionProfileControllerBuilder()
        .withLimits({1.0, 2.0, 10.0}) // vel, accel, jerk
        .withOutput(chassis)
        .buildMotionProfileController();

    // x, y, heading theta
    profileController->generatePath({
        {0.027466201353010256_ft, 0.14209244851244662_ft},
        {0.06469739701203414_ft, 0.3161556571185731_ft},
        {0.11022895703003943_ft, 0.51895048541523_ft},
        {0.162833225053995_ft, 0.7471066459691631_ft},
        {0.22128254473087194_ft, 0.9972538513471214_ft},
        {0.2843492597076356_ft, 1.266021814115847_ft},
        {0.3508057136312579_ft, 1.5500402468420926_ft},
        {0.419424250148706_ft, 1.8459388620926003_ft},
        {0.48897721290695007_ft, 2.1503473724341227_ft},
        {0.5582369455529577_ft, 2.4598954904334036_ft},
        {0.6259757917337012_ft, 2.771212928657187_ft},
        {0.6909660950961447_ft, 3.0809293996722236_ft},
        {0.7519801992872601_ft, 3.3856746160452653_ft},
        {0.8077904479540168_ft, 3.6820782903430507_ft},
        {0.8571691847433813_ft, 3.9667701351323306_ft},
        {0.898888753302324_ft, 4.23637986297985_ft},
        {0.931721497277815_ft, 4.487537186452355_ft},
        {0.9544397603168218_ft, 4.716871818116598_ft},
        {0.9658158860663129_ft, 4.921013470539323_ft},
        {0.9646222181732584_ft, 5.096591856287276_ft},
        {0.9496311002846274_ft, 5.240236687927204_ft},
        {0.917484678290347_ft, 5.352246279682509_ft},
        {0.8669698565259951_ft, 5.436903627851812_ft},
        {0.8000761163070216_ft, 5.4968154681147_ft},
        {0.7187929389488703_ft, 5.534588536150754_ft},
        {0.6251098057669884_ft, 5.552829567639553_ft},
        {0.5210161980768238_ft, 5.554145298260682_ft},
        {0.40850159719382084_ft, 5.541142463693722_ft},
        {0.2895554844334276_ft, 5.516427799618257_ft},
        {0.1661673411110911_ft, 5.482608041713867_ft},
        {0.04032664854225487_ft, 5.442289925660137_ft},
        {-0.08597711195763225_ft, 5.398080187136645_ft},
        {-0.2107544590731223_ft, 5.352585561822977_ft},
        {-0.3320159114887713_ft, 5.308412785398712_ft},
        {-0.44777198788913175_ft, 5.268168593543438_ft},
        {-0.5560332069587575_ft, 5.23445972193673_ft},
        {-0.6548100873822014_ft, 5.209892906258176_ft},
        {-0.7421131478440164_ft, 5.197074882187354_ft},
        {-0.8159529070287581_ft, 5.198612385403851_ft},
        {-0.8743398836209773_ft, 5.2171121515872425_ft},
        {-0.9152845963052307_ft, 5.25518091641712_ft},
        {-0.9367975637660684_ft, 5.315425415573056_ft},
        {-0.9358898243338065_ft, 5.394467862879343_ft},
        {-0.9125575862512334_ft, 5.486477946579914_ft},
        {-0.8692891230716109_ft, 5.590383613983814_ft},
        {-0.8085727083482017_ft, 5.705112812400094_ft},
        {-0.73289661563427_ft, 5.829593489137799_ft},
        {-0.6447491184830763_ft, 5.962753591505977_ft},
        {-0.5466184904478859_ft, 6.103521066813677_ft},
        {-0.4409930050819604_ft, 6.250823862369947_ft},
        {-0.330360935938562_ft, 6.403589925483834_ft},
        {-0.21721055657095611_ft, 6.560747203464386_ft},
        {-0.10403014053240245_ft, 6.721223643620651_ft},
        {0.006692038623834939_ft, 6.883947193261675_ft},
        {0.1124677073444924_ft, 7.047845799696508_ft},
        {0.2108085920763072_ft, 7.211847410234197_ft},
        {0.299226419266017_ft, 7.37487997218379_ft},
        {0.3752329153603595_ft, 7.535871432854334_ft},
        {0.4363398068060711_ft, 7.69374973955488_ft},
        {0.480058820049889_ft, 7.847442839594471_ft},
        {0.5039016815385509_ft, 7.995878680282157_ft},
        {0.5053801177187927_ft, 8.137985208926986_ft},
        {0.48200585503735205_ft, 8.272690372838005_ft},
        {0.48200585503735116_ft, 8.27269037283801_ft},
    }, "first");

	profileController->generatePath({
        {},
    }, "second");


    profileController->setTarget("first");

	cata.move_velocity(50);
	pros::delay(100);
	cata.move_velocity(0);

	profileController->waitUntilSettled();

	// profileController->setTarget("second");

    // profileController->waitUntilSettled();

}



void opcontrol() {
	leftDrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	rightDrive.set_brake_modes(E_MOTOR_BRAKE_COAST);
	cata.set_brake_mode(E_MOTOR_BRAKE_HOLD); // hold or coast?
	intake.set_brake_mode(E_MOTOR_BRAKE_COAST);

	Task checkControllerThread(checkController);

	lcd::register_btn0_cb([]{}); // put a callback in here
	lcd::register_btn1_cb([]{});
	lcd::register_btn2_cb([]{});

	while (true) {
		lcd::print(7, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int leftStick = master.get_analog(ANALOG_LEFT_Y);
		int rightStick = master.get_analog(ANALOG_RIGHT_Y);

		leftDrive.move(leftStick);
		rightDrive.move(rightStick);

		delay(20);
	}
}

