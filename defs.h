#ifndef _DEFS__H
#define _DEFS__H

#define WINDOW_TITLE "Spaceship War"
#define SCREEN_WIDTH 350
#define SCREEN_HEIGHT 750
#define INGAME_BACKGROUND_IMG "assets/background_ingame.png"
#define BLUE_SHIP_IMG "assets/blue_ship.png"
#define RED_SHIP_IMG "assets/red_ship.png"

const int INGAME_BACKGROUND_SCROLLING_SPEED = 1;

const int SHIP_WIDTH = 95;
const int SHIP_HEIGHT = 95;
const int SHIP_VELO = 5;

const int BLUE_SHIP_FIXED_COORDINATE_Y = 35;
const int RED_SHIP_FIXED_COORDINATE_Y = SCREEN_HEIGHT - SHIP_HEIGHT - 35;


#endif
