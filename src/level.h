#pragma once
#include "objects/object.h"
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <random>
#include <set>

struct Level {
  std::vector<std::unique_ptr<Object>> objects;
  std::jthread fixed_update_thread;
  std::chrono::high_resolution_clock::time_point fixed_update_start_time;
  std::vector<std::function<void(Level *)>> next_update;
  std::vector<std::tuple<std::string, double, Vec2D>> texts;
  std::mt19937_64 random_engine;
  std::set<Object *> to_remove;
  bool is_running;
  double draw_now;
  double last_fixed_update_time;
  double next_fixed_update_time;
  double fixed_updates_per_second_stat;
  long score;
  long id;

  Level(long id)
    : is_running(false), draw_now(0), last_fixed_update_time(0),
      next_fixed_update_time(0), fixed_updates_per_second_stat(100),
      score(0), id(id), random_engine(id) { }

  ~Level() {
    if (is_running)
      stop();
  }

  void start();

  void stop() {
    is_running = false;
    fixed_update_thread.join();
  }

  void addObject(std::unique_ptr<Object> obj) {
    objects.push_back(std::move(obj));
  }

  bool is_complete();

  void increase_score(long amount, Vec2D location);

  void draw();

  void update();

  void update_worker();
};