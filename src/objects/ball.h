#pragma once
#include "circle.h"

struct Ball : CircleObject {
  Ball(Level *level, double x, double y, double velX, double velY,
       const Image *img)
    : CircleObject(level, x, y, img) {
    velocity() = Vec2D(velX, velY);
  }

  void update() override;
};