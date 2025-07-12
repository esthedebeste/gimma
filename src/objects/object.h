#pragma once
#include <assert.h>
#include <simple-2d/image.h>
#include <simple-2d/shapes.h>

struct Level;

struct Object {
private:
  Vec2D m_previous_position, m_position, m_velocity;
  const Image *m_img;
  Level *m_level;

public:
  Object(Level *level, double x, double y, const Image *img)
    : m_previous_position(x, y), m_position(x, y), m_velocity(0, 0),
      m_img(img),
      m_level(level) { }

  virtual ~Object() = default;

  virtual bool collidingWith(Object &other) {
    return false; // Default implementation, should be overridden
  }

  Level *level() {
    assert(m_level != nullptr);
    return m_level;
  }

  Vec2D &position() {
    return m_position;
  }

  Vec2D &velocity() {
    return m_velocity;
  }

  const Image *&img() {
    return m_img;
  }

  double x() {
    return m_position.x();
  }

  double y() {
    return m_position.y();
  }

  double width() {
    return m_img->width;
  }

  double height() {
    return m_img->height;
  }

  virtual void draw();

  virtual void update();
};