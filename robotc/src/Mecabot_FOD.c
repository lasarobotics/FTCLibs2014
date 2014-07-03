#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking
#include "../lib/display.h" //splash screens

/***** STATICS *****/
static float k_deadband = 15;

/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = true; //true to enable competition mode

    displaySplash("Mecanum Bot", "FOD Test","");
    eraseDisplay();

    bool ok = false;
    while(!ok)
    {
        const int testcount = 2;
	    bool test[testcount] = {
	        errorcheck(1,1,MOTORCON),
	        errorcheck(1,2,MOTORCON)};
	    string desc[testcount] = {"MC1-1","MC1-2"};
	    ok = error_display(test,desc,testcount);
	    if (!ok) { PlayTone(440, 50); }
	    else { ClearSounds(); }
    }

    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{
    float leftFront, leftBack, rightFront, rightBack; // motors

    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
    StartTask(displaySmartDiags, 255);
    if (bCompetitionMode) {waitForStart();}

    while (true)
    {
        /***** Proportional Motor Control *****/
        getJoystickSettings(joystick); //get all joystick statuses

        mecanum_arcade(deadband(k_deadband,joystick.joy1_y1), deadband(k_deadband,joystick.joy1_x1), deadband(k_deadband,joystick.joy1_x2),
        leftFront, rightFront, leftBack, rightBack);

        motor[Lf] = leftFront;
        motor[Rf] = rightFront;
        motor[Lb] = leftBack;
        motor[Rb] = rightBack;
        nxtDisplayCenteredBigTextLine(6, "%.2f", gyro_getheading());
        nxtDisplayCenteredTextLine(5, "%.2f", gyro_getrotspeed());

        while(nNxtButtonPressed == kEnterButton) { gyro_reset(); }
    }
}
