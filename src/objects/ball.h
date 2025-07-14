#pragma once
#include "circle.h"

#include <set>

struct Ball : CircleObject {
private:
  int m_time_slow = 0;
  std::set<Object*> m_hit_pegs;
public:
  Ball(Level *level, double x, double y, double velX, double velY,
       const Image *img)
    : CircleObject(level, x, y, img) {
    velocity() = Vec2D(velX, velY);
  }

  void update() override;
};