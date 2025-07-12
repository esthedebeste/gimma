#pragma once
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