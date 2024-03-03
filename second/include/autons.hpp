#pragma once

#include "EZ-Template/drive/drive.hpp"
#include "EZ-Template/piston.hpp"

extern Drive chassis;

void default_constants();

void far_side_4_ball();
void far_side_6_ball();

void prog_skills();

void cata_process();

extern Piston front_left_wing;
extern Piston front_right_wing;
extern Piston back_left_wing;
extern Piston back_right_wing;
extern Piston elevation_1;
extern Piston elevation_2;

extern pros::Motor cata;
extern pros::Motor cata_2;
extern pros::Motor intake;

