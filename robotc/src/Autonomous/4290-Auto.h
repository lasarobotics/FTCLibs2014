#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTIRS2,             sensorI2CCustom)
#pragma config(Sensor, S4,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BlowerA,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BlowerB,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BlowerC,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_2,    BallStorage,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    GoalRetainer,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    Kickstand,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* Autonomous Program Headers for 4290 GiraPHPHe
* Contains methods for 4290 autonomous
**/

/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../../lib/naturalization.h" //naturalize RobotC
#include "../../lib/drive.h" //drive trains
#include "../../lib/i2c.h" //I2C error checking
#include "../../lib/options.h" //splash screens
#include "../../lib/ir.h" //other math

float getZone(float avgS2,float avgS3,bool newIR){
	float zone = 1;
	if ((avgS2 > 30) && (avgS3 > 30)) {
		zone = 2;
	}
	else if ((avgS2 < 10) && (avgS3 > 30)) {
		zone = 3;
	}
	nxtDisplayCenteredTextLine(3, "%i", zone);
	return zone;
}

/**
* Move down ramp
* Move the robot down the ramp
*/
void auto_moveDownRamp(){
	forward_Mecanum(3000, 100, 0, Lf, Lb, Rf, Rb);
}

/**
* Move from bottom of ramp to parking zone
*/
void auto_rampToParking(){
	forward_Mecanum(3000, 0, -100, Lf, Lb, Rf, Rb);
	forward_Mecanum(4250, -100, 0, Lf, Lb, Rf, Rb);
	forward_Mecanum(2500, 0, -100, Lf, Lb, Rf, Rb);
}
/**
* Center IR Right
* Move until the robot's gyro sensor is aligned to the goal.
*/
void centerIRRight(int zone){
	float leftFront, leftBack, rightFront, rightBack;
	//move until IR
	mecanum_arcade(0, -1, 0, leftFront, leftBack, rightFront, rightBack);
	motor[Lf] = leftFront*100;
	motor[Rf] = rightFront*100;
	motor[Lb] = leftBack*100;
	motor[Rb] = rightBack*100;
	while (ir_getavg(2) < 50 )
	{
		nxtDisplayCenteredTextLine(4, "IR2: %i", ir_getraw(2));
		nxtDisplayCenteredTextLine(5, "IR3: %i", ir_getraw(3));
		nxtDisplayCenteredTextLine(6, "Avg IR2: %i", ir_getavg(2));
		nxtDisplayCenteredTextLine(7, "Avg IR3: %i", ir_getavg(3));
	}
	//2.5in delay 250 for low bat
	forward_Mecanum(150, 0, -100, Lf, Lb, Rf, Rb);
	motor[Lf] = 0;
	motor[Rf] = 0;
	motor[Lb] = 0;
	motor[Rb] = 0;
	//Place ball sequence
	//~1200 for low bat
	forward_Mecanum(950, -25, 0, Lf, Lb, Rf, Rb);
}
/**
* Center IR
* Move until the robot's gyro sensor is aligned to the goal.
*/
void centerIRLeft(int zone){
float leftFront, leftBack, rightFront, rightBack;
	//move until IR
	mecanum_arcade(0, 1, 0, leftFront, leftBack, rightFront, rightBack);
	motor[Lf] = leftFront*100;
	motor[Rf] = rightFront*100;
	motor[Lb] = leftBack*100;
	motor[Rb] = rightBack*100;
	while (ir_getavg(3) < 50 )
	{
		nxtDisplayCenteredTextLine(4, "IR2: %i", ir_getraw(2));
		nxtDisplayCenteredTextLine(5, "IR3: %i", ir_getraw(3));
		nxtDisplayCenteredTextLine(6, "Avg IR2: %i", ir_getavg(2));
		nxtDisplayCenteredTextLine(7, "Avg IR3: %i", ir_getavg(3));
	}
	//2.5in delay 250 for low bat
	forward_Mecanum(150, 0, -100, Lf, Lb, Rf, Rb);
	motor[Lf] = 0;
	motor[Rf] = 0;
	motor[Lb] = 0;
	motor[Rb] = 0;
	//Place ball sequence
	//~1200 for low bat
	forward_Mecanum(950, 25, 0, Lf, Lb, Rf, Rb);
}
//TODO enum irAction

/***** PLACE IN CENTER GOAL *****/
// returns current zone (1,2,3)
float auto_placeCenterGoal(bool newIR)
{
	forward_encoder_Mecanum(-2000, -25, 0, Lf, Lb, Rf, Rb);
	//wait10Msec(30);
	//forward_Mecanum(400, 0, 100, Lf, Lb, Rf, Rb);
	//ir_wait(50); //wait 50 samples

	//Wait for a little bit
	wait1Msec(1000);
  float avgS2 = ir_getavg(2);
	float avgS3 = ir_getavg(3);
	float zone = getZone(avgS2,avgS3,newIR);
	zone = 3;

	/*if (zone == 3){
		centerIRRight(zone);
	}
	if (zone <= 2){
		//Nav to zone 2
		forward_Mecanum(1000, 0, -100, Lf, Lb, Rf, Rb);
		wait10Msec(100);
		turnToDeg_Mecanum(32, 100, Lf, Lb, Rf, Rb);
		wait1Msec(100);
		if (zone == 2) { centerIRLeft(zone); }
	}
	if (zone == 1){
		//Nav to zone 1 (farthest)
		forward_Mecanum(1000, 0, 100, Lf, Lb, Rf, Rb);
		wait10Msec(100);
		turnToDeg_Mecanum(90, 70, Lf, Lb, Rf, Rb);
		wait1Msec(100);
		forward_Mecanum(2100, 0, 100, Lf, Lb, Rf, Rb);
		centerIRLeft(zone);
	}*/

	return zone;
}

/**** PLACE IN KICKSTAND
(from already placing a ball in center goal) ****/
void auto_kickstandFromCenterGoal(int zone)
{
	//hook the goal
	wait1Msec(250);
	forward_Mecanum(1300, 0, 100, Lf, Lb, Rf, Rb);
	wait1Msec(250);
	forward_Mecanum(600, 100, 0, Lf, Lb, Rf, Rb);
	wait1Msec(250);
	forward_Mecanum(1000, 0, -100, Lf, Lb, Rf, Rb); //hit the goal
	wait1Msec(250);
	forward_Mecanum(2200, 100, 0, Lf, Lb, Rf, Rb); //continue moving
}
