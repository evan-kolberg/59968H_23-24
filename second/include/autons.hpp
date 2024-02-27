#pragma once

#include "EZ-Template/drive/drive.hpp"
#include "EZ-Template/piston.hpp"

extern Drive chassis;

void drive_example();
void turn_example();
void drive_and_turn();
void wait_until_change_speed();
void swing_example();
void combining_movements();
void interfered_example();
void default_constants();

void far_side_4_ball();
void far_side_6_ball();

void close_side();
void prog_skills();

void cata_process();

extern Piston front_left_wing;
extern Piston front_right_wing;
extern Piston back_left_wing;
extern Piston back_right_wing;
extern Piston elevation_1;
extern Piston elevation_2;

extern pros::Motor cata;
extern pros::Motor intake;

