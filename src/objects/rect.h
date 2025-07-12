#pragma once
#include "object.h"

struct RectObject : Object {
  RectObject(Level *level, double x, double y, const Image *img)
    : Object(level, x, y, img) { }

  bool collidingWith(Object &other) override;

  void draw() override;
};