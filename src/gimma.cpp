#include "consts.h"
#include "images.h"
#include "level.h"
#include "objects/ball.h"
#include "objects/object.h"
#include "objects/peg.h"
#include "objects/rect.h"
#include "resources.h"
#include <algorithm>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <numbers>
#include <numeric>
#include <simple-2d/2d.h>
#include <simple-2d/image.h>
#include <simple-2d/setup.h>
#include <thread>

size_t level_index = 0;
Level *curr_level = nullptr;
std::vector<std::unique_ptr<Level>> levels{};

int width = 800;
int height = 600;
auto title = "Ballenbak gimma";

void mouse_pressed(Mouse button, KeyMods mods);
void fixed_update();

void init(Callbacks &on) {
  on.key_held = [](const Key key, KeyMods mods) {
    if (key == Key::ESCAPE) {
      exit(0);
    }
  };
  on.mouse_pressed = mouse_pressed;
}

void first_update() {
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
  glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glad_glEnable(GL_BLEND);

  levels = std::vector<std::unique_ptr<Level>>{};
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>());
    bool flip = false;
    for (int y = 32; y < 300; y += 64) {
      for (int x = flip ? 32 : 64; x < 800 - 64; x += 64) {
        level.addObject(std::make_unique<Peg>(&level, x, y));
      }
      flip = !flip;
    }
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>());
    bool flip = false;
    for (int y = 32; y < 300; y += 64) {
      if (flip)
        for (int x = 32; x < 800; x += 64)
          level.addObject(std::make_unique<Peg>(&level, x, y));
      else
        for (int x = 0; x < 800; x += 128)
          level.addObject(
              std::make_unique<RectObject>(&level, x, y, &small_rectangle_img));
      flip = !flip;
    }
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>());

    level.addObject(std::make_unique<Peg>(&level, 468, 115));
    level.addObject(std::make_unique<Peg>(&level, 447, 118));
    level.addObject(std::make_unique<Peg>(&level, 412, 118));
    level.addObject(std::make_unique<Peg>(&level, 380, 115));
    level.addObject(std::make_unique<Peg>(&level, 342, 118));
    level.addObject(std::make_unique<Peg>(&level, 495, 122));
    level.addObject(std::make_unique<Peg>(&level, 335, 129));
    level.addObject(std::make_unique<Peg>(&level, 314, 129));
    level.addObject(std::make_unique<Peg>(&level, 272, 150));
    level.addObject(std::make_unique<Peg>(&level, 285, 140));
    level.addObject(std::make_unique<Peg>(&level, 265, 150));
    level.addObject(std::make_unique<Peg>(&level, 244, 160));
    level.addObject(std::make_unique<Peg>(&level, 233, 171));
    level.addObject(std::make_unique<Peg>(&level, 216, 185));
    level.addObject(std::make_unique<Peg>(&level, 198, 195));
    level.addObject(std::make_unique<Peg>(&level, 181, 209));
    level.addObject(std::make_unique<Peg>(&level, 310, 398));
    level.addObject(std::make_unique<Peg>(&level, 303, 381));
    level.addObject(std::make_unique<Peg>(&level, 300, 374));
    level.addObject(std::make_unique<Peg>(&level, 293, 353));
    level.addObject(std::make_unique<Peg>(&level, 289, 332));
    level.addObject(std::make_unique<Peg>(&level, 286, 315));
    level.addObject(std::make_unique<Peg>(&level, 531, 405));
    level.addObject(std::make_unique<Peg>(&level, 524, 385));
    level.addObject(std::make_unique<Peg>(&level, 524, 374));
    level.addObject(std::make_unique<Peg>(&level, 524, 357));
    level.addObject(std::make_unique<Peg>(&level, 524, 336));
    level.addObject(std::make_unique<Peg>(&level, 524, 318));
    level.addObject(std::make_unique<Peg>(&level, 443, 325));
    level.addObject(std::make_unique<Peg>(&level, 419, 290));
    level.addObject(std::make_unique<Peg>(&level, 412, 283));
    level.addObject(std::make_unique<Peg>(&level, 401, 273));
    level.addObject(std::make_unique<Peg>(&level, 384, 248));
    level.addObject(std::make_unique<Peg>(&level, 373, 238));
    level.addObject(std::make_unique<Peg>(&level, 366, 224));
    level.addObject(std::make_unique<Peg>(&level, 380, 217));
    level.addObject(std::make_unique<Peg>(&level, 397, 217));
    level.addObject(std::make_unique<Peg>(&level, 422, 217));
    level.addObject(std::make_unique<Peg>(&level, 436, 213));
    level.addObject(std::make_unique<Peg>(&level, 450, 213));
    level.addObject(std::make_unique<Peg>(&level, 412, 301));
    level.addObject(std::make_unique<Peg>(&level, 418, 301));
    level.addObject(std::make_unique<Peg>(&level, 422, 307));
  }

  level_index = 0;
  curr_level = levels[level_index].get();
  curr_level->start();
}

int framecount = 0;

void update() {
  framecount++;
  if (framecount == 1)
    first_update();

  curr_level->draw();

  if (curr_level->is_complete()) {
    curr_level->stop();
    level_index++;
    if (level_index < levels.size()) {
      curr_level = levels[level_index].get();
      curr_level->start();
    } else {
      std::cout << "All levels completed! Success!" << std::endl;
      exit(0);
    }
  }
}

void shoot() {
  double gunX = width / 2;
  double gunY = height;
  double angleToMouse =
      std::atan2(mouse_y - gunY, mouse_x - gunX) / std::numbers::pi * 180;

  double velX = std::cos(angleToMouse * std::numbers::pi / 180);
  double velY = std::sin(angleToMouse * std::numbers::pi / 180);

  curr_level->next_update.emplace_back([=](Level *level) {
    level->addObject(
        std::make_unique<Ball>(level, gunX + velX * 40, gunY + velY * 40,
                               velX * BALL_SHOOTING_STRENGTH,
                               velY * BALL_SHOOTING_STRENGTH, &ball_img));
  });
}

void mouse_pressed(Mouse button, KeyMods mods) {
  if (button == Mouse::LEFT) {
    shoot();
  }
}