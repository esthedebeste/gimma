#pragma once
#include "object.h"

struct CircleObject : Object {
  CircleObject(Level *level, double cx, double cy, const Image *img)
    : Object(level, cx - img->width / 2, cy - img->height / 2, img) { }

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