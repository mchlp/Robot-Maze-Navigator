#ifndef UTIL_H_
#define UTIL_H_

#include <main.h>

struct u_motor {
  unsigned char channel;
  bool reversed;
  double mul;
  double off;
  double min;
  double max;
};

struct u_pid {
  double kp, ki, kd;
  double sumErr, lastErr;
};

struct u_motor_module {
  struct u_motor motor;
  struct u_pid pid;
  Encoder *encoder;
};

struct u_motor_drive_group {
  struct u_motor_module fl;
  struct u_motor_module fr;
  struct u_motor_module bl;
  struct u_motor_module br;
};

static double u_clamp(double value, double min, double max) {
  return value < min ? min : value > max ? max : value;
}

static void u_write_motor(struct u_motor *motor, double value) {
  int speed = u_clamp(value * motor->mul, motor->min, motor->max) * MAX_MOTOR_SPEED;
  if (motor->reversed) {
    speed *= -1;
  }
  printf("%d\n", speed);
  motorSet(motor->channel, speed);
}

static double u_pid_update(struct u_pid *pid, double want, double actual) {
  double error = want - actual;
  pid->sumErr += error;
  double pPart = error * pid->kp;
  double iPart = pid->sumErr * pid->ki;
  double dPart = (error - pid->lastErr) * pid->kd;
  pid->lastErr = error;
  double corr = pPart + iPart + dPart;
  return corr;
}

static void u_write_motor_pid(struct u_motor_module *module, double value) {
  double ev = encoderGet(module->encoder);
  encoderReset(module->encoder);
  u_write_motor(&module->motor, u_pid_update(&module->pid, value, ev));
}

static void u_write_motor_drive(struct u_motor_drive_group *group, double forward, double right) {
  int efl, efr, ebl, ebr;
  efl = encoderGet(group->fl.encoder);
  efr = encoderGet(group->fr.encoder);
  ebl = encoderGet(group->bl.encoder);
  ebr = encoderGet(group->br.encoder);
  encoderReset(group->fl.encoder);
  encoderReset(group->fr.encoder);
  encoderReset(group->bl.encoder);
  encoderReset(group->br.encoder);
  int el = efl > ebl ? efl : ebl;
  int er = efr > ebr ? efr : ebr;
  int middle = (el + er) / 2;

  u_write_motor_pid(&group->fl, middle + right);
  u_write_motor_pid(&group->fr, middle - right);
  u_write_motor_pid(&group->bl, middle + right);
  u_write_motor_pid(&group->br, middle - right);
}

static double u_get_joy_analog(unsigned int joystick, unsigned int axis) {
  return joystickGetAnalog(joystick, axis) / (double) MAX_MOTOR_SPEED;
}

#endif // UTIL_H_
