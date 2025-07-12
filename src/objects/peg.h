#pragma once
#include "circle.h"

struct Peg : CircleObject {
private:
  bool m_hit = false;
public:

  Peg(Level* level, double x, double y);

  void hit();
  bool is_hit() const {
    return m_hit;
  }
  void draw() override;
};