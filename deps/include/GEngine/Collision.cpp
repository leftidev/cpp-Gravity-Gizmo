#include "Collision.h"

#include <iostream>

namespace GEngine {

namespace Collision {

void Circle::getCollider(int width)
{
    // Set collision circle bounds.
    radius = width / 2;
};

void Rect::getCollider(int _x, int _y, int _width, int _height)
{
    // Set rect collision bounds.
    x = _x;
    y = _y;

    width = _width;
    height = _height;
};

bool checkCircleVSCircle(Circle& a, Circle& b)

{
    // Calculate total radius squared.
    int totalRadiusSquared = a.radius + b.radius;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    // If the distance between the centers of the circles is less than the sum of their radii:
    if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
    {
        // The circles have collided.
        return true;
    }

    // If no collision.
    return false;
}

bool checkCircleVSRect(Circle& circle, Rect& rect)
{
    // Closest point on collision box.
    int cX, cY;

    // Find closest x offset.
    if (circle.x < rect.x)
    {
        cX = rect.x;
    }
    else if (circle.x > rect.x + rect.width)
    {
        cX = rect.x + rect.width;
    }
    else
    {
        cX = circle.x;
    }

    // Find closest y offset.
    if (circle.y < rect.y)
    {
        cY = rect.y;
    }
    else if (circle.y > rect.y + rect.height)
    {
        cY = rect.y + rect.height;
    }
    else
    {
        cY = circle.y;
    }

    // If the closest point is inside the circle.
    if (distanceSquared(circle.x, circle.y, cX, cY) < circle.radius * circle.radius)
    {
        // This box and the circle have collided.
        return true;
    }

    // If the shapes have not collided.
    return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;

    return deltaX*deltaX + deltaY*deltaY;
}

}
}