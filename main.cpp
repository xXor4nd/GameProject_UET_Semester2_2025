#include "game.h"
#include "gameloop.h"

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    Graphics graphics;
    graphics.init();

    Asset assets(graphics);
    assets.loadAllAssets();

    Sound sounds(graphics, assets);

    graphics.prepareScene();
    Game SpaceshipWar(graphics, assets, sounds);
    gameLoop(SpaceshipWar);

    assets.clean();
    graphics.quit();

    return 0;
}
