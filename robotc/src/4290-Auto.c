#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S4,     HTGYRO,         sensorAnalogInactive)
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
#pragma config(Servo,  srvo_S2_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    BallStorage,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    Kickstand,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    TouchSensor,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    GoalRetainer,         tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* Autonomous Program for 4290 GiraPHPHe
* Moves from parking zone, places ball into small goal, then executes center goal scoring
**/
/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/lego-touch.h"
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/logging.h" //logging
#include "../lib/drive.h" //drive trains
#include "../lib/options.h" //splash screens
#include "../lib/sensor.h" //sensor IO
#include "Autonomous/4290-Auto.h" //naturalize RobotC
const tMUXSensor irSensor =  msensor_S3_2;
const tMUXSensor touchSensorOne = msensor_S3_3;
void init()
{
  bSmartDiagnostics = true; //true to enable smart diagnostic screen
  bCompetitionMode = true; //true to enable competition mode
  displaySplash("GiraPHPHe", "Autonomous", true);
  eraseDisplay();
  gyro_init(HTGYRO);
  ir_init(irSensor);
  touch_init(touchSensorOne);
  wait1Msec(50);
  nxtbarOn();

  options_reset();

  options_create(0, "START");
  options_add(0, "Parking");
  options_add(0, "Ramp");


  options_create(1, "MOVE");
  options_add(1, "to Block");
  options_add(1, "to 90cm");
  options_add(1, "No");

  options_create(2, "90 DIR");
  options_add(2, "Default");
  options_add(2, "Left");
  options_add(2, "Right");

  options_create(3, "WAIT");
  options_add(3, "0 s");
  options_add(3, "5 s");
  options_add(3, "10 s");

  options_create(4, "LOGGING");
  options_add(4, "On");
  options_add(4, "Off");

  options_display("LASA 4290","READY!");
  wait10Msec(100);
  //STORE OPTIONS DETAILS
  //if logging is on
  if (options_get[4] == 0) { log_enabled = true; }
  return;
}

task main()
{
  init();
  servo[BallStorage] = 140;
  servo[GoalRetainer] = 140;
  servo[Kickstand] = 155;
  servo[TouchSensor] = 65;
  StartTask(readSensors);
  StartTask(displaySmartDiags);
  if (bCompetitionMode) {waitForStart();}
  ClearTimer(T4);
  auto_init();

  //WAIT if requested
  if (options_get[3] != 0) { wait10Msec(500 * options_get[3]); }

  if (options_get[0] == 0){
    //True for new IR, false for old
    float zone = auto_placeCenterGoal(true);
    if (options_get[1] == 1) { auto_centerGoalToLarge(zone,options_get[2]); }
  }
  else if (options_get[0] == 1){
    auto_moveDownRamp();
  }
  //kill everything
  servo[BallStorage] = BallStorage_Open;
  log_stop();
  StopAllTasks();
}
