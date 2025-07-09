#include <numeric>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <thread>
#include <simple-2d/2d.h>
#include <simple-2d/image.h>
#include <simple-2d/setup.h>
#include "resources.h"

constexpr bool LOG_FPS = false;
constexpr bool DEBUG_PHYSICS_MODE = false;
constexpr double TARGET_UPS = 100.0;
// The speed of the ball when shot
constexpr double BALL_SHOOTING_STRENGTH = 7;
// Gravity applied to the ball every update
constexpr double BALL_GRAVITY = 0.15;
// Friction applied to the ball's velocity every update
constexpr double BALL_AIR_FRICTION = 1 - 0.005;
// Energy loss on bounce
constexpr double BALL_ENERGY_LOSS = 0.7;

Image load_resource_image(const unsigned char *data, unsigned size) {
  const std::vector image_data(data, data + size);
  auto img = decode_image(image_data);
  if (!img) {
    std::cerr << "Failed to load image from resource data" << std::endl;
    exit(1);
  }
  return img.value();
}

Image hit_peg_img = load_resource_image(RESOURCES_hit_peg_png,
                                        RESOURCES_hit_peg_png_size);
Image peg_img = load_resource_image(RESOURCES_peg_png,
                                    RESOURCES_peg_png_size);
Image rectangle_img = load_resource_image(RESOURCES_rect_png,
                                          RESOURCES_rect_png_size);
Image gun_img = load_resource_image(RESOURCES_gun_png,
                                    RESOURCES_gun_png_size);
Image ball_img = load_resource_image(RESOURCES_ball_png,
                                     RESOURCES_ball_png_size);

double draw_now;
double last_fixed_update_time;
double next_fixed_update_time;

double lerp(double from, double to, double start, double end, double now) {
  return from + (to - from) * (now - start) / (end - start);
}

struct Object {
private:
  Vec2D m_previous_position, m_position, m_velocity;
  const Image *m_img;

public:
  Object(double x, double y, const Image *img)
    : m_previous_position(x, y), m_position(x, y), m_velocity(0, 0),
      m_img(img) { }

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

  double draw_x() {
    return lerp(m_previous_position.x(), x(), last_fixed_update_time,
                next_fixed_update_time, draw_now);
  }

  double draw_y() {
    return lerp(m_previous_position.y(), y(), last_fixed_update_time,
                next_fixed_update_time, draw_now);
  }

  double width() {
    return m_img->width;
  }

  double height() {
    return m_img->height;
  }

  virtual ~Object() = default;

  virtual bool collidingWith(Object &other) {
    return false; // Default implementation, should be overridden
  }

  virtual void draw() {
    color(1, 1, 1, 1);
    if (DEBUG_PHYSICS_MODE)
      image(*img(), x(), y());
    else
      image(*img(), draw_x(), draw_y());
  }

  virtual void update() {
    m_previous_position = m_position;
    position() = position() + velocity();
  }
};

struct CircleObject : Object {
  CircleObject(double x, double y, const Image *img)
    : Object(x, y, img) { }

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

  void draw() override {
    Object::draw();
    if (DEBUG_PHYSICS_MODE) {
      color(0.6, 0.2, 0.2, 0.6);
      circle(cx(), cy(), radius());
    }
  }
};

struct Peg : CircleObject {
  bool hit = false;

  Peg(double x, double y)
    : CircleObject(x, y, &peg_img) { }

  void draw() override {
    if (hit)
      img() = &hit_peg_img;
    else
      img() = &peg_img;
    CircleObject::draw();
  }
};

struct RectObject : Object {

  RectObject(double x, double y, const Image *img)
    : Object(x, y, img) { }

  bool collidingWith(Object &other) override;

  void draw() override {
    Object::draw();

    if (DEBUG_PHYSICS_MODE) {
      color(0.6, 0.2, 0.2, 0.6);
      rect(x(), y(), width(), height());
    }
  }
};

bool CircleObject::collidingWith(Object &other) {
  if (auto c = dynamic_cast<CircleObject *>(&other)) {
    double dx = cx() - c->cx();
    double dy = cy() - c->cy();
    double distanceSquared = dx * dx + dy * dy;
    double radiusSum = radius() + c->radius();
    return distanceSquared <= radiusSum * radiusSum;
  }
  if (auto r = dynamic_cast<RectObject *>(&other)) {
    double closestX = std::max(r->x(), std::min(cx(), r->x() + r->width()));
    double closestY = std::max(r->y(), std::min(cy(), r->y() + r->height()));
    double dx = cx() - closestX;
    double dy = cy() - closestY;
    return (dx * dx + dy * dy) <= (radius() * radius());
  }
  throw std::runtime_error(
      "Unsupported collision check with non-physical object");
}

bool RectObject::collidingWith(Object &other) {
  if (auto c = dynamic_cast<CircleObject *>(&other)) {
    double closestX = std::max(x(), std::min(c->cx(), x() + width()));
    double closestY = std::max(y(), std::min(c->cy(), y() + height()));
    double dx = c->cx() - closestX;
    double dy = c->cy() - closestY;
    return (dx * dx + dy * dy) <= (c->radius() * c->radius());
  }
  if (auto r = dynamic_cast<RectObject *>(&other)) {
    return !(x() + width() < r->x()
             || x() > r->x() + r->width()
             || y() + height() < r->y()
             || y() > r->y() + r->height()
    );
  }
  throw std::runtime_error(
      "Unsupported collision check with non-physical object");
}

struct Level;

struct Ball : CircleObject {
  Level *level;

  Ball(double x, double y, double velX, double velY, const Image *img,
       Level *level)
    : CircleObject(x, y, img), level(level) {
    velocity() = Vec2D(velX, velY);
  }

  void update() override;
};

struct Level {
  std::vector<std::unique_ptr<Object>> objects;

  void addObject(std::unique_ptr<Object> obj) {
    objects.push_back(std::move(obj));
  }

  bool checkCollisions(Object &obj) {
    for (auto &other : objects)
      if (obj.collidingWith(*other))
        return true;
    return false;
  }

  void draw() {
    for (auto &obj : objects)
      if (obj != nullptr)
        obj->draw();
  }

  void update() {
    for (auto &obj : objects)
      obj->update();
    for (auto it = objects.begin(); it != objects.end();) {
      if ((*it)->y() < -(*it)->height()) {
        // Remove things in the void
        it = objects.erase(it);
      } else {
        ++it;
      }
    }
  }
};

void Ball::update() {
  CircleObject::update();
  velocity().y(velocity().y() - BALL_GRAVITY);
  velocity() = velocity() * BALL_AIR_FRICTION;

  double origMagnitude = velocity().distance(Vec2D(0, 0));
  // Check for collisions with other objects
  for (auto &other : level->objects) {
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
      velocity().x(dx / distance * origMagnitude);
      velocity().y(dy / distance * origMagnitude);
      if (auto peg = dynamic_cast<Peg *>(c)) {
        peg->hit = true;
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

Level level{};

std::thread fixed_update_thread;

int width = 800;
int height = 600;
auto title = "Ballenbak gimma";

void mouse_pressed(Mouse button, KeyMods mods);
double fixed_updates_per_second_stat = 100.0;
void fixed_update();

std::chrono::high_resolution_clock::time_point fixed_update_start_time;

bool is_running = true;

void init(Callbacks &on) {
  on.key_held = [](const Key key, KeyMods mods) {
    if (key == Key::ESCAPE) {
      is_running = false;
      exit(0);
    }
  };

  bool flip = false;
  for (int y = 32; y < 300; y += 64) {
    for (int x = flip ? 32 : 0; x < 800; x += 64) {
      level.addObject(std::make_unique<Peg>(x, y));
    }
    flip = !flip;
  }
  on.mouse_pressed = mouse_pressed;
}

void first_update() {
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
  glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glad_glEnable(GL_BLEND);

  fixed_update_thread = std::thread([] {
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
      fixed_update();
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
  });
  std::atexit([] {
    is_running = false;
    if (fixed_update_thread.joinable())
      fixed_update_thread.join();
  });
}

int framecount = 0;


std::vector<std::function<void()>> next_fixed_update;


void fixed_update() {
  for (auto &func : next_fixed_update)
    func();
  next_fixed_update.clear();
  level.update();
}


void update() {
  draw_now = std::chrono::duration_cast<std::chrono::duration<double>>(
          std::chrono::high_resolution_clock::now() - fixed_update_start_time).
      count();
  double fps = 1 / delta_time;
  if (LOG_FPS)
    std::cout << "\rFPS: " << fps << ", UPS: "
        << fixed_updates_per_second_stat << ", Last fixed update: " <<
        last_fixed_update_time
        << ", Next fixed update: " << next_fixed_update_time
        << ", Draw now: " << draw_now;
  framecount++;
  if (framecount == 1)
    first_update();
  background(175, 25, 175);
  level.draw();

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

  color(0, 0, 0, 1);
  circle(mouse_x, mouse_y, 2);
}

void shoot() {
  double gunX = width / 2;
  double gunY = height;
  double angleToMouse = std::atan2(mouse_y - gunY, mouse_x - gunX) /
                        std::numbers::pi * 180;

  double velX = std::cos(angleToMouse * std::numbers::pi / 180);
  double velY = std::sin(angleToMouse * std::numbers::pi / 180);

  next_fixed_update.emplace_back([=] {
    level.addObject(std::make_unique<Ball>(gunX + velX * 40, gunY + velY * 40,
                                           velX * BALL_SHOOTING_STRENGTH,
                                           velY * BALL_SHOOTING_STRENGTH,
                                           &ball_img,
                                           &level));
  });
}

void mouse_pressed(Mouse button, KeyMods mods) {
  if (button == Mouse::LEFT) {
    shoot();
  }
}