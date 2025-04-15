#include "game.h"

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    Graphics graphics;
    graphics.init();

    Asset assets(graphics);
    assets.loadAllAssets();

    Sound sounds(graphics, assets);

    ScrollingBackground bgr(graphics, assets);
    BlueShip blueShip(graphics, assets, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT);
    RedShip redShip(graphics, assets, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d);
    Bullet bullet(graphics, assets, sounds);

    graphics.prepareScene();

    gameLoop(graphics, assets, sounds, bgr, blueShip, redShip, bullet);

    assets.clean();
    graphics.quit();

    return 0;
}
