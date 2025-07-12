#include "simple-2d/2d.h"
#include "object.h"
#include "../consts.h"
#include "../level.h"

double lerp(double from, double to, double start, double end, double now) {
  return from + (to - from) * (now - start) / (end - start);
}

void Object::draw() {
  auto draw_x = lerp(m_previous_position.x(), x(),
                     level()->last_fixed_update_time,
                     level()->next_fixed_update_time, level()->draw_now);
  auto draw_y = lerp(m_previous_position.y(), y(),
                     level()->last_fixed_update_time,
                     level()->next_fixed_update_time, level()->draw_now);
  if (DEBUG_PHYSICS_MODE) {
    draw_x = x();
    draw_y = y();
  }
  color(1, 1, 1, 1);
  image(*img(), draw_x, draw_y);
}

void Object::update() {
  m_previous_position = m_position;
  position() = position() + velocity();
}