#include "map.h"

Map::Map(int x0, int y0, int z0)
{
    maxX = x0; maxY = y0; maxZ = z0;
    minX = 0; minY = 3; minZ = 0;
}

void Map::createMap(std::vector < std::vector<std::vector<bool>>>& mass) {
    Image im;  im.loadFromFile("resources/heightmap.png");


    for (int x = 0; x < 256; x++)
        for (int z = 0; z < 256; z++)
        {
            int c = im.getPixel(x, z).r / 15;
            for (int y = 0; y < c; y++)
                if (y > c - 3) mass[x][y][z] = 1;
        }
}

bool Map::check(int x, int y, int z, std::vector < std::vector<std::vector<bool>>>& mass)
{
    if ((x < 0) || (x >= 500) ||
        (y < 0) || (y >= 500) ||
        (z < 0) || (z >= 500)) return false;

    return mass[x][y][z];
}

void Map::drawMap(TextureManager textureManager, float size, GLuint box, std::vector < std::vector<std::vector<bool>>>& mass) {
    for (int x = minX; x < maxX; x++)
        for (int y = minY; y < maxY; y++)
            for (int z = minZ; z < maxZ; z++)
            {
                if (!check(x, y, z, mass)) continue;

                glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);

                textureManager.drawBox(box, size / 2);

                glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
            }

}