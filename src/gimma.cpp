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
    Level &level = *levels.emplace_back(std::make_unique<Level>(0));
    bool flip = false;
    for (int y = 64; y < 300; y += 80) {
      for (int x = flip ? 32 : 16; x < 800; x += 64) {
        level.addObject(std::make_unique<Peg>(&level, x, y));
      }
      flip = !flip;
    }
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>(1));
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
    Level &level = *levels.emplace_back(std::make_unique<Level>(2));

    level.addObject(std::make_unique<Peg>(&level, 468, 115));
    level.addObject(std::make_unique<Peg>(&level, 447, 118));
    level.addObject(std::make_unique<Peg>(&level, 412, 118));
    level.addObject(std::make_unique<Peg>(&level, 380, 115));
    level.addObject(std::make_unique<Peg>(&level, 342, 118));
    level.addObject(std::make_unique<Peg>(&level, 495, 122));
    level.addObject(std::make_unique<Peg>(&level, 314, 129));
    level.addObject(std::make_unique<Peg>(&level, 250, 150));
    level.addObject(std::make_unique<Peg>(&level, 285, 140));
    level.addObject(std::make_unique<Peg>(&level, 233, 171));
    level.addObject(std::make_unique<Peg>(&level, 216, 185));
    level.addObject(std::make_unique<Peg>(&level, 198, 195));
    level.addObject(std::make_unique<Peg>(&level, 181, 209));
    level.addObject(std::make_unique<Peg>(&level, 310, 398));
    level.addObject(std::make_unique<Peg>(&level, 300, 374));
    level.addObject(std::make_unique<Peg>(&level, 289, 332));
    level.addObject(std::make_unique<Peg>(&level, 286, 315));
    level.addObject(std::make_unique<Peg>(&level, 531, 405));
    level.addObject(std::make_unique<Peg>(&level, 524, 374));
    level.addObject(std::make_unique<Peg>(&level, 524, 357));
    level.addObject(std::make_unique<Peg>(&level, 524, 336));
    level.addObject(std::make_unique<Peg>(&level, 524, 318));
    level.addObject(std::make_unique<Peg>(&level, 443, 325));
    level.addObject(std::make_unique<Peg>(&level, 419, 290));
    level.addObject(std::make_unique<Peg>(&level, 401, 273));
    level.addObject(std::make_unique<Peg>(&level, 384, 248));
    level.addObject(std::make_unique<Peg>(&level, 366, 224));
    level.addObject(std::make_unique<Peg>(&level, 420, 213));
    level.addObject(std::make_unique<Peg>(&level, 450, 213));
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>(3));

    level.addObject(std::make_unique<Peg>(&level, 38, 211));
    level.addObject(std::make_unique<Peg>(&level, 73, 173));
    level.addObject(std::make_unique<Peg>(&level, 129, 142));
    level.addObject(std::make_unique<Peg>(&level, 178, 124));
    level.addObject(std::make_unique<Peg>(&level, 227, 114));
    level.addObject(std::make_unique<Peg>(&level, 276, 107));
    level.addObject(std::make_unique<Peg>(&level, 325, 103));
    level.addObject(std::make_unique<Peg>(&level, 371, 106));
    level.addObject(std::make_unique<Peg>(&level, 423, 110));
    level.addObject(std::make_unique<Peg>(&level, 469, 120));
    level.addObject(std::make_unique<Peg>(&level, 511, 134));
    level.addObject(std::make_unique<Peg>(&level, 552, 159));
    level.addObject(std::make_unique<Peg>(&level, 584, 173));
    level.addObject(std::make_unique<Peg>(&level, 626, 152));
    level.addObject(std::make_unique<Peg>(&level, 657, 135));
    level.addObject(std::make_unique<Peg>(&level, 689, 114));
    level.addObject(std::make_unique<Peg>(&level, 720, 103));
    level.addObject(std::make_unique<Peg>(&level, 755, 96));
    level.addObject(std::make_unique<Peg>(&level, 101, 159));
    level.addObject(std::make_unique<Peg>(&level, 115, 309));
    level.addObject(std::make_unique<Peg>(&level, 132, 236));
    level.addObject(std::make_unique<Peg>(&level, 223, 278));
    level.addObject(std::make_unique<Peg>(&level, 273, 204));
    level.addObject(std::make_unique<Peg>(&level, 357, 271));
    level.addObject(std::make_unique<Peg>(&level, 409, 212));
    level.addObject(std::make_unique<Peg>(&level, 486, 288));
    level.addObject(std::make_unique<Peg>(&level, 546, 219));
    level.addObject(std::make_unique<Peg>(&level, 626, 281));
    level.addObject(std::make_unique<Peg>(&level, 650, 208));
    level.addObject(std::make_unique<Peg>(&level, 727, 240));
    level.addObject(std::make_unique<Peg>(&level, 756, 169));
    level.addObject(std::make_unique<Peg>(&level, 346, 173));
    level.addObject(std::make_unique<Peg>(&level, 287, 271));
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>(4));

    level.addObject(std::make_unique<Peg>(&level, 268, 444));
    level.addObject(std::make_unique<Peg>(&level, 243, 406));
    level.addObject(std::make_unique<Peg>(&level, 219, 368));
    level.addObject(std::make_unique<Peg>(&level, 198, 325));
    level.addObject(std::make_unique<Peg>(&level, 187, 273));
    level.addObject(std::make_unique<Peg>(&level, 190, 221));
    level.addObject(std::make_unique<Peg>(&level, 208, 179));
    level.addObject(std::make_unique<Peg>(&level, 232, 140));
    level.addObject(std::make_unique<Peg>(&level, 257, 116));
    level.addObject(std::make_unique<Peg>(&level, 281, 95));
    level.addObject(std::make_unique<Peg>(&level, 313, 77));
    level.addObject(std::make_unique<Peg>(&level, 344, 67));
    level.addObject(std::make_unique<Peg>(&level, 379, 63));
    level.addObject(std::make_unique<Peg>(&level, 418, 63));
    level.addObject(std::make_unique<Peg>(&level, 449, 63));
    level.addObject(std::make_unique<Peg>(&level, 488, 66));
    level.addObject(std::make_unique<Peg>(&level, 530, 80));
    level.addObject(std::make_unique<Peg>(&level, 561, 101));
    level.addObject(std::make_unique<Peg>(&level, 593, 122));
    level.addObject(std::make_unique<Peg>(&level, 617, 143));
    level.addObject(std::make_unique<Peg>(&level, 631, 178));
    level.addObject(std::make_unique<Peg>(&level, 625, 214));
    level.addObject(std::make_unique<Peg>(&level, 621, 249));
    level.addObject(std::make_unique<Peg>(&level, 611, 284));
    level.addObject(std::make_unique<Peg>(&level, 600, 326));
    level.addObject(std::make_unique<Peg>(&level, 586, 364));
    level.addObject(std::make_unique<Peg>(&level, 572, 402));
    level.addObject(std::make_unique<Peg>(&level, 554, 434));
  }
  {
    Level &level = *levels.emplace_back(std::make_unique<Level>(5));
    level.addObject(std::make_unique<Peg>(&level, 692, 541));
    level.addObject(std::make_unique<Peg>(&level, 730, 507));
    level.addObject(std::make_unique<Peg>(&level, 741, 457));
    level.addObject(std::make_unique<Peg>(&level, 713, 419));
    level.addObject(std::make_unique<Peg>(&level, 664, 416));
    level.addObject(std::make_unique<Peg>(&level, 409, 363));
    level.addObject(std::make_unique<Peg>(&level, 451, 307));
    level.addObject(std::make_unique<Peg>(&level, 482, 367));
    level.addObject(std::make_unique<Peg>(&level, 660, 282));
    level.addObject(std::make_unique<Peg>(&level, 702, 227));
    level.addObject(std::make_unique<Peg>(&level, 619, 216));
    level.addObject(std::make_unique<Peg>(&level, 499, 188));
    level.addObject(std::make_unique<Peg>(&level, 429, 188));
    level.addObject(std::make_unique<Peg>(&level, 475, 121));
    level.addObject(std::make_unique<Peg>(&level, 293, 128));
    level.addObject(std::make_unique<Peg>(&level, 251, 195));
    level.addObject(std::make_unique<Peg>(&level, 328, 192));
    level.addObject(std::make_unique<Peg>(&level, 363, 297));
    level.addObject(std::make_unique<Peg>(&level, 300, 370));
    level.addObject(std::make_unique<Peg>(&level, 234, 311));
    level.addObject(std::make_unique<Peg>(&level, 48, 429));
    level.addObject(std::make_unique<Peg>(&level, 51, 374));
    level.addObject(std::make_unique<Peg>(&level, 83, 328));
    level.addObject(std::make_unique<Peg>(&level, 128, 293));
    level.addObject(std::make_unique<Peg>(&level, 94, 377));
    level.addObject(std::make_unique<Peg>(&level, 38, 181));
    level.addObject(std::make_unique<Peg>(&level, 90, 118));
    level.addObject(std::make_unique<Peg>(&level, 188, 66));
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
#ifndef NDEBUG
  if (button == Mouse::RIGHT) {
    curr_level->next_update.emplace_back([](Level *level) {
      level->addObject(std::make_unique<Peg>(level, mouse_x, mouse_y));
      std::cout << "level.addObject(std::make_unique<Peg>(&level, " << mouse_x
          << ", " << mouse_y << "));" << std::endl;
    });
  }
#endif
}