#include "rect.h"
#include "../consts.h"
#include "circle.h"
#include "object.h"
#include <simple-2d/2d.h>
#include <simple-2d/setup.h>

bool RectObject::collidingWith(Object &other) {
  if (auto c = dynamic_cast<CircleObject *>(&other)) {
    double closestX = std::max(x(), std::min(c->cx(), x() + width()));
    double closestY = std::max(y(), std::min(c->cy(), y() + height()));
    double dx = c->cx() - closestX;
    double dy = c->cy() - closestY;
    return (dx * dx + dy * dy) <= (c->radius() * c->radius());
  }
  if (auto r = dynamic_cast<RectObject *>(&other)) {
    return !(x() + width() < r->x()
             || x() > r->x() + r->width()
             || y() + height() < r->y()
             || y() > r->y() + r->height()
    );
  }
  throw std::runtime_error(
      "Unsupported collision check with non-physical object");
}

void RectObject::draw() {
  Object::draw();

  if (DEBUG_PHYSICS_MODE) {
    color(0.6, 0.2, 0.2, 0.6);
    rect(x(), y(), width(), height());
  }
}