#include "peg.h"
#include "circle.h"
#include "../images.h"
#include "../level.h"

Peg::Peg(Level *level, double x, double y)
  : CircleObject(level, x, y, &peg_img) { }

void Peg::hit() {
  if (!m_hit)
    level()->increase_score(10, Vec2D{x() + radius() / 2, cy() + radius()});
  m_hit = true;
}

void Peg::draw() {
  if (m_required && !m_hit)
    img() = &required_peg_img;
  else if (m_required && m_hit)
    img() = &required_hit_peg_img;
  else if (!m_required && !m_hit)
    img() = &peg_img;
  else if (!m_required && m_hit)
    img() = &hit_peg_img;
  CircleObject::draw();
}