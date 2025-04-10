#ifndef _DEFS__H
#define _DEFS__H

#define WINDOW_TITLE "Spaceship War"
#define SCREEN_WIDTH 350
#define SCREEN_HEIGHT 750
#define INGAME_BACKGROUND_IMG "assets/background_ingame.png"
#define BLUE_SHIP_IMG "assets/blue_ship.png"
#define RED_SHIP_IMG "assets/red_ship.png"
#define BULLET_IMG "assets/bullet.png"
#define HEART_IMG "assets/Heart.png"
#define HEALTH_BAR_IMG "assets/health_bar.png"
#define TIME_FONT "assets/Purisa-BoldOblique.ttf"

const int INGAME_BACKGROUND_SCROLLING_SPEED = 1;
const int INGAME_BACKGROUND_WIDTH = 350;
const int INGAME_BACKGROUND_HEIGHT = 750;

const int SHIP_WIDTH = 95;
const int SHIP_HEIGHT = 95;
const int SHIP_VELO = 3;

const int BLUE_SHIP_FIXED_COORDINATE_Y = 35;
const int RED_SHIP_FIXED_COORDINATE_Y = SCREEN_HEIGHT - SHIP_HEIGHT - 35;

const int BLUE_SHIP_RESTRICTED_LINE_X = 0;
const int BLUE_SHIP_RESTRICTED_LINE_Y = 120;
const int RED_SHIP_RESTRICTED_LINE_X = 0;
const int RED_SHIP_RESTRICTED_LINE_Y = SCREEN_HEIGHT - BLUE_SHIP_RESTRICTED_LINE_Y;

const int BULLET_WIDTH = 19;
const int BULLET_HEIGHT = 19;

const int countdownTime = 3000;
int startTime = 0;

const int HEART_WIDTH = 25;
const int HEART_HEIGHT = 25;
const int HEALTH_BAR_WIDTH = 240;
const int HEALTH_BAR_HEIGHT = 25;

const int BLUE_SHIP_HEART_FIXED_COORDINATE_X = 20;
const int BLUE_SHIP_HEART_FIXED_COORDINATE_Y = 5;
const int BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_X = 60;
const int BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y = 5;

const int RED_SHIP_HEART_FIXED_COORDINATE_X = 20;
const int RED_SHIP_HEART_FIXED_COORDINATE_Y = SCREEN_HEIGHT - BLUE_SHIP_FIXED_COORDINATE_Y;
const int RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_X = 60;
const int RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y = SCREEN_HEIGHT - BLUE_SHIP_FIXED_COORDINATE_Y;

#endif
