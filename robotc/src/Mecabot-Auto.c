#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     HTIRS2,             sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
 * Autonomous Program for Mecabot NoStep
 * Moves from parking zone, places ball into small goal, then executes center goal scoring
 **/

/***** INCLUDES *****/
#include "Autonomous/Mecabot-Auto.h" //naturalize RobotC

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("NoStep", "IR Seek Test", true);

    bool ok = true;
    while(!ok)
    {
        ok = errorcheck(1,0,1,MOTORCON) &&
        		 errorcheck(1,0,2,MOTORCON);
		    if (!ok) {
		        PlayTone(440, 50);
		        nxtDisplayCenteredBigTextLine(3, "ERROR!");
		    }
		    else { ClearSounds(); nxtDisplayCenteredBigTextLine(3, "A OK!"); }
    }

    eraseDisplay();
    gyro_init(HTGYRO);
    ir_init(HTIRS2);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{
    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
    StartTask(ir_calibrate,8);
    StartTask(displaySmartDiags, 255);
    auto_moveDownRamp();
    auto_rampToParking();
    //True for new IR, false for old
    float zone = auto_placeCenterGoal(false);
    //auto_kickstandFromCenterGoal(zone);
    //kill everything
    StopAllTasks();
}
