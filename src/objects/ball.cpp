#include "ball.h"
#include "../level.h"
#include "peg.h"
#include "rect.h"
#include "../consts.h"
#include "simple-2d/setup.h"

void Ball::update() {
  CircleObject::update();
  const double origMagnitude = velocity().distance(Vec2D(0, 0));
  if (origMagnitude < 10 / TARGET_UPS)
    m_time_slow++;
  velocity().y(velocity().y() - BALL_GRAVITY);
  velocity() = velocity() * BALL_AIR_FRICTION;

  if (y() < -height())
    m_time_slow += 5;
  if (m_time_slow > 100) {
    for (auto &peg : m_hit_pegs) {
      level()->to_remove.insert(peg);
    }
    level()->to_remove.insert(this);
    return;
  }
  // Check for collisions with other objects
  for (auto &other : level()->objects) {
    if (this == other.get())
      continue; // Skip self
    if (!collidingWith(*other))
      continue;
    // now for a good bounce, we see what kind of object we hit
    if (auto c = dynamic_cast<CircleObject *>(other.get())) {
      // Bounce off circle
      double dx = cx() - c->cx();
      double dy = cy() - c->cy();
      double distance = std::sqrt(dx * dx + dy * dy);
      if (distance == 0)
        continue; // Avoid division by zero
      const double origAngle = std::atan2(velocity().y(), velocity().x());
      const double reflectAngle = std::atan2(-dx, dy);
      const double angleDiff = reflectAngle - origAngle;
      // Reflect the velocity vector
      const double newAngle = origAngle + 2 * angleDiff;
      Vec2D reflected(std::cos(newAngle), std::sin(newAngle));
      Vec2D bounced(dx / distance, dy / distance);
      velocity() = reflected * origMagnitude * 0.8 + bounced * origMagnitude *
                   0.2;
      if (auto peg = dynamic_cast<Peg *>(c)) {
        peg->hit();
        m_hit_pegs.insert(peg);
      }
    } else if (auto r = dynamic_cast<RectObject *>(other.get())) {
      // Bounce off rectangle
      double closestX = std::max(r->x(), std::min(cx(), r->x() + r->width()));
      double closestY = std::max(r->y(), std::min(cy(), r->y() + r->height()));
      double cappedMagnitude = std::min(origMagnitude, r->width() / 2);
      if (closestX <= r->x() + origMagnitude) {
        velocity().x(-std::abs(velocity().x()));
      } else if (closestX >= r->x() + r->width() - cappedMagnitude) {
        velocity().x(std::abs(velocity().x()));
      }
      if (closestY <= r->y() + cappedMagnitude) {
        velocity().y(-std::abs(velocity().y()));
      } else if (closestY >= r->y() + r->height() - cappedMagnitude) {
        velocity().y(std::abs(velocity().y()));
      }
    }
    if (velocity().distanceSq(Vec2D(0, 0)) > 1)
      while (collidingWith(*other)) {
        // Move out of collision
        position().x(position().x() + velocity().x() * 0.1);
        position().y(position().y() + velocity().y() * 0.1);
      }
    velocity() = velocity() * BALL_ENERGY_LOSS; // lose some energy
    break;
  }

  // Check for wall collisions
  if (x() < 0 || x() + width() > ::width) {
    if (x() < 0)
      velocity().x(std::abs(velocity().x()));
    else if (x() + width() > ::width)
      velocity().x(-std::abs(velocity().x()));
    velocity() = velocity() * BALL_ENERGY_LOSS; // lose some energy
  }
}