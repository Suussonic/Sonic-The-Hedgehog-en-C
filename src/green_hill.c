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
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 3584, 112), 3584, 0, -2, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 365, 3584, 144), 3584, 112, -2, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 3584, 112), 7168, 0, -2, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 365, 3584, 144), 7168, 112, -2, 0);
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


    map_add_collision(getPos(2818, 16, 256, 20), 0);
    map_add_collision(getPos(3074, -60, 300, 76), 0);
    map_add_slope(3400, -55, 3560, -130, 0, 20);
    map_add_collision(getPos(3560, -130, 80, 20), 0);

    map_add_collision(getPos(3330, 130, 328, 20), 0);
    map_add_slope(3658, 130, 3800, 200, 0, 20);
    map_add_collision(getPos(3800, 200, 328, 20), 0);
    map_add_slope(4128, 200, 4270, 130, 0, 20);
    map_add_collision(getPos(4270, 130, 434, 20), 0);

    map_add_collision(getPos(4700, 95, 68, 35), 0);
    map_add_collision(getPos(4766, 62, 162, 34), 0);
    map_add_collision(getPos(4928, 28, 322, 48), 0);
    map_add_collision(getPos(5250, 68, 192, 20), 0);


    map_add_collision(getPos(3840, -240, 192, 20), 0);
    map_add_slope(3900, -90, 4010, -120, 0, 20);
    map_add_collision(getPos(4032, -120, 70, 20), 0);

    map_add_collision(getPos(4290, -120, 190, 20), 0);
    map_add_collision(getPos(4480, -150, 62, 30), 0);
    map_add_collision(getPos(4544, -190, 94, 40), 0);
    map_add_slope(4638, -190, 4828, -120, 0, 20);
    map_add_collision(getPos(4828, -120, 120, 20), 0);
    map_add_slope(4948, -110, 5070, -58, 0, 20);
    map_add_collision(getPos(5070, -58, 590, 20), 0);
    map_add_slope(5660, -58, 5820, -125, 0, 20);
    map_add_collision(getPos(5820, -125, 370, 20), 0);

    map_add_collision(getPos(6300, 130, 420, 20), 0);

    map_add_collision(getPos(6830, 390, 150, 20), 0);
    map_add_slope(6960, 390, 7137, 460, 0, 20);
    map_add_collision(getPos(7137, 450, 80, 20), 0);
    map_add_slope(7220, 450, 7280, 400, 0, 20);
    map_add_collision(getPos(7300, 390, 380, 20), 0);
    map_add_collision(getPos(7680, 450, 870, 20), 0);
    map_add_collision(getPos(8540, 420, 100, 30), 0);
    map_add_collision(getPos(8640, 390, 390, 30), 0);
    map_add_slope(9030, 390, 9180, 460, 0, 20);
    map_add_collision(getPos(9180, 460, 1062, 20), 0);


    map_add(OBJECTS, getPos(24, 485, 32, 32), 150, 160, 0, 1);
    map_add(OBJECTS, getPos(24, 198, 16, 16), 400, 160, 0, 1);

    return 10240;
}