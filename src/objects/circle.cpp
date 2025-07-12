#include "circle.h"

#include "rect.h"
#include "../consts.h"
#include <simple-2d/2d.h>

void CircleObject::draw() {
  Object::draw();
  if (DEBUG_PHYSICS_MODE) {
    color(0.6, 0.2, 0.2, 0.6);
    circle(cx(), cy(), radius());
  }
}

bool CircleObject::collidingWith(Object &other) {
  if (auto c = dynamic_cast<CircleObject *>(&other)) {
    double dx = cx() - c->cx();
    double dy = cy() - c->cy();
    double distanceSquared = dx * dx + dy * dy;
    double radiusSum = radius() + c->radius();
    return distanceSquared <= radiusSum * radiusSum;
  }
  if (auto r = dynamic_cast<RectObject *>(&other)) {
    double closestX = std::max(r->x(), std::min(cx(), r->x() + r->width()));
    double closestY = std::max(r->y(), std::min(cy(), r->y() + r->height()));
    double dx = cx() - closestX;
    double dy = cy() - closestY;
    return (dx * dx + dy * dy) <= (radius() * radius());
  }
  throw std::runtime_error(
      "Unsupported collision check with non-physical object");
}