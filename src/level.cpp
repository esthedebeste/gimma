#include "level.h"

#include "consts.h"
#include "drawutils.h"
#include "images.h"
#include "objects/peg.h"
#include "simple-2d/2d.h"
#include "simple-2d/setup.h"

#include <iostream>
#include <numbers>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <deque>
#include <set>


void Level::draw() {
  if (!is_running)
    return; // Don't draw if not running
  draw_now = std::chrono::duration_cast<std::chrono::duration<double>>(
          std::chrono::high_resolution_clock::now() - fixed_update_start_time)
      .
      count();
  double fps = 1 / delta_time;
  if (LOG_FPS)
    std::cout << "\rFPS: " << fps << ", UPS: "
        << fixed_updates_per_second_stat << ", Last fixed update: " <<
        last_fixed_update_time
        << ", Next fixed update: " << next_fixed_update_time
        << ", Draw now: " << draw_now;
  background(1, 1, 1);

  color(1, 1, 1, 1);
  push_matrix();
  translate(width / 2, height);
  double gunWidth = gun_img.width;
  double gunHeight = gun_img.height;
  double gunX = width / 2;
  double gunY = height;
  double angleToMouse = std::atan2(mouse_y - gunY, mouse_x - gunX) /
                        std::numbers::pi * 180;
  rotate(angleToMouse + 90);
  image(gun_img, -gunWidth / 2, -gunHeight / 2);
  pop_matrix();
  double velX = std::cos(angleToMouse / 180 * std::numbers::pi);
  double velY = std::sin(angleToMouse / 180 * std::numbers::pi);
  double prevX = ::width / 2 + velX * 40;
  double prevY = ::height + velY * 40;
  velX *= BALL_SHOOTING_STRENGTH;
  velY *= BALL_SHOOTING_STRENGTH;
  for (int i = 0; i < 100; ++i) {
    double x = prevX + velX;
    double y = prevY + velY;
    color(1, 0.2, 0.2, 0.2);
    line_width(3.5);
    if (i % 5 == 4)
      line(prevX, prevY, x, y);
    velY -= BALL_GRAVITY;
    velX *= BALL_AIR_FRICTION;
    velY *= BALL_AIR_FRICTION;
    prevX = x;
    prevY = y;
  }

  color(0, 0, 0, 1);
  circle(mouse_x, mouse_y, 2);
  for (auto &obj : objects)
    if (obj != nullptr)
      obj->draw();
  for (auto it = texts.begin(); it != texts.end();) {
    auto &[str, expiration_time, pos] = *it;
    double time_to = expiration_time - draw_now;
    if (time_to <= 0) {
      // Remove expired text
      it = texts.erase(it);
    } else {
      color(0.2, 1, 0.2);
      text(str, pos.x(), pos.y() - time_to * 20, 1.5);
      ++it;
    }
  }
  std::string score_text = std::to_string(score);
  color(1, 0, 1);
  text(score_text, 10, height - 74, 4);
}

void Level::update() {
  for (auto &func : next_update)
    func(this);
  next_update.clear();
  for (auto &obj : objects)
    obj->update();
  for (auto it = objects.begin(); it != objects.end();) {
    if (to_remove.contains(it->get())) {
      it = objects.erase(it);
    } else {
      ++it;
    }
  }
  to_remove.clear();
}

void Level::update_worker() {
  fixed_update_start_time =
      std::chrono::high_resolution_clock::now();
  constexpr auto ms =
      std::chrono::duration_cast<
        std::chrono::high_resolution_clock::duration>(
          std::chrono::duration<double>(1) / TARGET_UPS);
  auto target_time = fixed_update_start_time;
  std::deque update_times{fixed_update_start_time};
  while (is_running) {
    target_time += ms;
    update();
    auto now = std::chrono::high_resolution_clock::now();
    last_fixed_update_time = std::chrono::duration_cast<std::chrono::duration<
      double>>(now - fixed_update_start_time).count();
    next_fixed_update_time = std::chrono::duration_cast<std::chrono::duration<
      double>>(now + ms - fixed_update_start_time).count();
    update_times.push_back(now);
    while (now - update_times.front() > std::chrono::duration<double>(1))
      update_times.pop_front();
    fixed_updates_per_second_stat = update_times.size();
    std::this_thread::sleep_until(target_time);
  }
}

void Level::start() {
  is_running = true;
  fixed_update_thread = std::jthread([this] {
    return update_worker();
  });

  random_engine.seed(std::random_device()());
  std::bernoulli_distribution bernoulli_dist(0.3);
  for (auto &obj : objects)
    if (const auto peg = dynamic_cast<Peg *>(obj.get())) {
      peg->set_required(bernoulli_dist(random_engine));
    }
}

bool Level::is_complete() {
  if (!texts.empty())
    return false;
  for (auto &obj : objects)
    if (const auto peg = dynamic_cast<Peg *>(obj.get()))
      if (peg->is_required() && !peg->is_hit())
        return false; // Still pegs left
  return true; // All pegs hit
}

void Level::increase_score(long amount, Vec2D location) {
  score += amount;
  texts.emplace_back(std::to_string(amount), last_fixed_update_time + 1,
                     location);
}