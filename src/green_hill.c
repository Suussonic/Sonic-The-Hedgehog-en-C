#include <green_hill.h>
#include <maputils.h>

int load_stage() {
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 3584, 112), 0, 0, -2, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 365, 3584, 144), 0, 112, -2, 0);
    map_add(GREEN_HILL_FOREGROUND, getPos(24, 264, 10240, 1280), 0, -765, -1, 0);

    map_add(BADNIKS, getPos(173, 275, 48, 32), 200, 140, 0, 1);

    map_add_collision(getPos(0, 200, 856, 20), 0);

    return 10240;
}