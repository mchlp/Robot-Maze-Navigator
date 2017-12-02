#ifndef UTIL_H_
#define UTIL_H_

#include <main.h>

struct u_motor {
  unsigned char channel;
  double mul;
  double off;
  double min;
  double max;
};

struct u_motor_drive_group {
  struct u_motor fl;
  struct u_motor fr;
  struct u_motor bl;
  struct u_motor br;
};

static double u_clamp(double value, double min, double max) {
  return value < min ? min : value > max ? max : value;
}

static void u_write_motor(struct u_motor *motor, double value) {
  int speed = u_clamp(value * motor->mul, -motor->min, -motor->max) * MAX_MOTOR_SPEED;
  motorSet(motor->channel, speed);
}

static void u_write_motor_drive(struct u_motor_drive_group *group, double forward, double right) {
  u_write_motor(&group->fl, forward + right);
  u_write_motor(&group->fr, forward - right);
  u_write_motor(&group->bl, forward + right);
  u_write_motor(&group->br, forward - right);
}

static double u_get_joy_analog(unsigned int joystick, unsigned int axis) {
  return joystickGetAnalog(joystick, axis);
}

#endif // UTIL_H_
