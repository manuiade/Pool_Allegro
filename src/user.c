//-----------------------------------------------------------------------------
// USER.C - contains definition of USER_TASK and all its auxiliar functions
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
// LOCAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
int mousePos, xMouse, yMouse;                   //used to calc mouse position
uint8_t prev_mouse_state;                       //used to check mouse click
uint8_t curr_mouse_state;                       //used to check mouse click

bool load_flag;                                 //true if cue is loading

int left_balls = N_BALLS;                       //used to check remaining balls
//-----------------------------------------------------------------------------
// GET_MOUSE_POSITION FUNCTION - calculate x and y mouse coordinates
//-----------------------------------------------------------------------------
void    get_mouse_position() {
    mousePos = mouse_pos;
	xMouse = mousePos >> 16;
	yMouse = mousePos & 0x0000ffff;
}
//-----------------------------------------------------------------------------
// CALCULATE_AIM_ANGLE FUNCTION - calculate aim angle based on mouse position
//-----------------------------------------------------------------------------
double  calculate_aim_angle() {
    point   p3;                                 //stores mouse projection
    double  distance;                           //dist between mouse and cue

    distance = get_dist(&p3, xMouse, yMouse, ball[0].c.x, ball[0].c.y);
    return get_angle(p3, distance);             //return calculated angle
}
//-----------------------------------------------------------------------------
// CALCULATE_CUE_ANGLE FUNCTION - calculate cue angle in range 0-255 and pivot
// in order to rotate cue bitmap along centre of white ball as pivot
//-----------------------------------------------------------------------------
void    calculate_cue_angle() {
    //get cue angle
    user.cue_angle = ((user.aim_angle + 90 ) * 256 / 360);
    //update pivot based on angle
    user.p.x = (ball[0].c.x + cos((user.aim_angle + 270) * PI / 180) * 6);
	user.p.y = (ball[0].c.y + sin((user.aim_angle + 270) * PI / 180) * 6);
}
//-----------------------------------------------------------------------------
// CHECK_MOUSE_CLICK FUNCTION - check if user left click on mouse (toggle mode)
//-----------------------------------------------------------------------------
void    check_mouse_click() {
	if (mouse_b & 1)
        curr_mouse_state = 1;
    if (curr_mouse_state && ! prev_mouse_state)
        user.state += 1;
    prev_mouse_state = curr_mouse_state;
    curr_mouse_state = 0;
}
//-----------------------------------------------------------------------------
// LOAD_RELEASE_CUE FUNCTION - moves cue back and forth white ball, calculating
// the show power inversely proportional to the distance 
//-----------------------------------------------------------------------------
void    load_release_cue() {
	if (!load_flag)                            //moves back
		user.wd -= SPEED / 4;
	else                                       //moves forth
		user.wd += SPEED / 4;
    //boundaries to invert movement
	if (user.wd < -160 || user.wd > -20)
		load_flag = !load_flag;
    //calculate shot power
	user.shot_power = ((double) abs(user.wd) / 160.0f) * SPEED;
}
//-----------------------------------------------------------------------------
// RELEASE_CUE FUNCTION - moves cue towards white ball, telling user task
// to move it once the cue reaches the ball 
//-----------------------------------------------------------------------------
void    release_cue() {
	if (user.wd < -15)
		user.wd += SPEED * 2;
	else
		user.state = WAKE_BALL;
}
