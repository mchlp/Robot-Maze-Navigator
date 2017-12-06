/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

static struct u_motor_drive_group driveGroup = {
	{{M_DRIVE_FL, true, 1, 0, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED}, {0.05, 0, 0, 0, 0}, &drive_elf},
	{{M_DRIVE_FR, false, 1, 0, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED}, {0.05, 0, 0, 0, 0}, &drive_erf},
	{{M_DRIVE_BL, true, 1, 0, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED}, {0.05, 0, 0, 0, 0}, &drive_elb},
	{{M_DRIVE_BR, false, 1, 0, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED}, {0.05, 0, 0, 0, 0}, &drive_erb}
};

void operatorControl() {
	while (1) {

		double forward, right;
		forward = u_get_joy_analog(JOYSTICK_PRIMARY, J_DRIVE_FB);
		right = u_get_joy_analog(JOYSTICK_PRIMARY, J_DRIVE_LR);

		printf("JOYSTICK STATUS: %f %f\n", forward, right);

		/*
		u_write_motor(&(driveGroup.fl.motor), (forward + right)*127);
	  u_write_motor(&(driveGroup.fr.motor), (forward - right)*127);
	  u_write_motor(&(driveGroup.bl.motor), (forward + right)*127);
	  u_write_motor(&(driveGroup.br.motor), (forward - right)*127);
		*/

		motorSet(M_DRIVE_FL, (forward + right)*127);
		motorSet(M_DRIVE_FR, -(forward - right)*127);
		motorSet(M_DRIVE_BL, (forward + right)*127);
		motorSet(M_DRIVE_BR, -(forward - right)*127);

		delay(20);
	}
}
