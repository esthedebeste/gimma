#pragma once
#include "circle.h"

struct Peg : CircleObject {
private:
  bool m_hit = false;
  bool m_required = false;

public:
  Peg(Level *level, double x, double y);

  void hit();

  bool is_hit() const {
    return m_hit;
  }

  void set_required(const bool required = true) {
    m_required = required;
  }

  bool is_required() const {
    return m_required;
  }

  void draw() override;
};