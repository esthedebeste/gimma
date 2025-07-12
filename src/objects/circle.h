#pragma once
#include "object.h"

struct CircleObject : Object {
  CircleObject(Level *level, double x, double y, const Image *img)
    : Object(level, x, y, img) { }

  double radius() {
    return width() / 2;
  }

  double cx() {
    return x() + radius();
  }

  double cy() {
    return y() + radius();
  }

  bool collidingWith(Object &other) override;

  void draw() override;
};