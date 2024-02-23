#pragma once

#include "EZ-Template/drive/drive.hpp"

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
void close_side();
void skills();

extern ez::Piston front_left_wing;
extern ez::Piston front_right_wing;
extern ez::Piston back_left_wing;
extern ez::Piston back_right_wing;
extern ez::Piston elevation_1;
extern ez::Piston elevation_2;

extern pros::Motor cata;
extern pros::Motor intake;

