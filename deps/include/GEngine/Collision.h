#pragma once

namespace GEngine {

namespace Collision {

class Circle
{
public:
    int x, y;
    int radius;

    void getCollider(int width);
};

class Rect
{
public:
    int x, y;
    int height, width;

    void getCollider(int x, int y, int width, int height);
};

bool checkCircleVSCircle(Circle& a, Circle& b);

bool checkCircleVSRect(Circle& a, Rect& b);

double distanceSquared(int x1, int y1, int x2, int y2);

}
}