#include <green_hill.h>
#include <maputils.h>

void map_add_slope(int x0, int y0, int x1, int y1, int z, int size) {
        double slope = (double)(y1 - y0) / (x1 - x0);
        int amt = (x1 - x0) / size;

        for (int i = 0; i <= amt; i++) {
            int x = x0 + i * size;
            int y = y0 + (int)(slope * (x - x0));
            map_add_collision(getPos(x, y, size, size), z);
        }
        map_add_collision(getPos(x1, y1, size, size), z);
    }

int load_stage() {
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 3584, 112), 0, 0, -2, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 365, 3584, 144), 0, 112, -2, 0);
    map_add(GREEN_HILL_FOREGROUND, getPos(24, 264, 10240, 1280), 0, -765, -1, 0);

    map_add(BADNIKS, getPos(173, 275, 48, 32), 200, 160, 0, 1);

    map_add_collision(getPos(0, 200, 856, 20), 0);

    map_add_collision(getPos(856, 190, 20, 20), 0);
    map_add_slope(856, 190, 1000, 125, 0, 20);
    map_add_collision(getPos(1000, 125, 90, 20), 0);

    map_add_collision(getPos(1280, 125, 320, 20), 0);
    map_add_slope(1600, 125, 1700, 180, 0, 20);
    map_add_slope(1700, 180, 1760, 200, 0, 20);
    map_add_slope(1760, 200, 1820, 190, 0, 20);

    map_add_slope(1840, 190, 1870, 180, 0, 20);
    map_add_slope(1880, 170, 1890, 160, 0, 20);
    map_add_collision(getPos(1895, 150, 20, 20), 0);

    map_add_collision(getPos(1900, 130, 274, 20), 0);
    map_add_collision(getPos(2174, 100, 64, 34), 0);
    map_add_collision(getPos(2238, 66, 130, 36), 0);
    map_add_collision(getPos(2368, 16, 256, 50), 0);


    map_add(OBJECTS, getPos(24, 485, 32, 32), 150, 160, 0, 1);
    map_add(OBJECTS, getPos(24, 198, 16, 16), 400, 160, 0, 1);

    return 10240;
}