#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S3,     HTSMUX,              sensorI2CCustom)
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
#pragma config(Servo,  srvo_S2_C1_2,    BallStorage,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    GoalRetainer,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    Kickstand,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    TouchSensor,          tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/**
* Autonomous Program Headers for 4290 GiraPHPHe
* Contains methods for 4290 autonomous
**/
const int BallStorage_Closed = 140;
const int BallStorage_OpenSmall = 100;
const int BallStorage_Open = 80;
const int GoalRetainer_Open = 130;
const int GoalRetainer_Closed = 25;

void auto_init()
{
    if (log_enabled)
    {
        log_init("auto.txt",false);
        log_write("GEN","AUTO LOG: Started");
    }
}

float getZone(float avgS2,float avgS3,bool newIR){
    float zone = 1;
    if ((avgS2 > 20) && (avgS3 > 20)) {
        zone = 2;
    }
    else if ((avgS2 < 10) && (avgS3 > 20)) {
        zone = 3;
    }

        nxtDisplayTextLine(6, "%f", avgS3);
    string strtemp;
    StringFormat(strtemp,"Z%f %1.2f %1.2f",zone,avgS2,avgS3);
    string blah = "";
    StringFormat(blah,"%f",zone);
    log_write("IR",blah);
    StringFormat(blah,"%f",avgS2);
    log_write("IR",blah);
    StringFormat(blah,"%f",avgS3);
    log_write("IR",blah);
    return zone;
}

void move_encoderortouch(float encodercount, float forward, float strafe, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
    nMotorEncoder[Rb] = 0;
    float leftFront, leftBack, rightFront, rightBack;
    forward = speedController(forward);
    strafe = speedController(strafe);
    mecanum_arcade(forward, strafe, 0,
    leftFront, rightFront, leftBack, rightBack);

    //Move
    motor[Lf] = leftFront*100;
    motor[Rf] = rightFront*100;
    motor[Lb] = leftBack*100;
    motor[Rb] = rightBack*100;
    while (abs(nMotorEncoder[Rb]) <= encodercount){
        if (get_touch(1) || get_touch(2)){
            nxtDisplayCenteredTextLine(7, "PRESSED");
            break;
        }
    }
    //Stop
    nMotorEncoder[Rb] = 0;
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
    motor[Rb] = 0;
}
/**
* Move down ramp
* Move the robot down the ramp
*/
void auto_moveDownRamp(){
    forward_encoderMecanum(4000, 0, 50, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(4000, 0, 40, Lf, Lb, Rf, Rb);
}

/**
* Move from bottom of ramp to parking zone
*/
void auto_rampToParking(){
    forward_encoderMecanum(3000, 0, -100, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(4250, -100, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(2500, 0, -100, Lf, Lb, Rf, Rb);
}
/**
* Center IR Right
* Move until the robot's gyro sensor is aligned to the goal.
*/
void centerIRRight(int zone){
    float leftFront, leftBack, rightFront, rightBack;
    //move until IR
    mecanum_arcade(0, -.5, 0, leftFront, leftBack, rightFront, rightBack);
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
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
    motor[Rb] = 0;
    forward_encoderMecanum(600,0,-25,Lf,Lb,Rf,Rb);
    wait1Msec(1000);
    //Place ball sequence
    forward_encoderMecanum(800, -25, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(400, -15, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(500, -5, 0, Lf, Lb, Rf, Rb);
    wait1Msec(1000);
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
    forward_encoderMecanum(150, 0, -100, Lf, Lb, Rf, Rb);
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
    motor[Rb] = 0;
    //Place ball sequence
    //~1200 for low bat
    forward_encoderMecanum(950, 25, 0, Lf, Lb, Rf, Rb);
}
//TODO enum irAction

const float kwait = 1.00;

/***** PLACE IN CENTER GOAL *****/
// returns current zone (1,2,3)
float auto_placeCenterGoal(bool newIR)
{
    servo[GoalRetainer] = 255;
    servo[TouchSensor] = 120;
    wait1Msec(500);
    motor[Intake] = 30;
    wait1Msec(2000);
    motor[Intake] = 1;
    forward_encoderMecanum(2100, -40, 0, Lf, Lb, Rf, Rb);
    //Wait for a little bit
    wait1Msec(500);
    gyro_reset();

    float avgS2 = ir_getavg(2);
    float avgS3 = ir_getavg(3);
    float zone = getZone(avgS2,avgS3,newIR);



    zone = 2;




    if (zone == 3){
        servo[TouchSensor] = 190;
        forward_encoderMecanum(1100, 0, -100, Lf, Lb, Rf, Rb);
        wait1Msec(500);
        move_encoderortouch(2000, -50, 0, Lf, Lb, Rf, Rb);
        move_encoderortouch(267, -25, 0, Lf, Lb, Rf, Rb);
        forward_encoderMecanum(500, 0, -100, Lf, Lb, Rf, Rb);
    }
    if (zone == 2){
        servo[TouchSensor] = 190;
        wait1Msec(500);
        //Place ball sequence

       //TODO:USE TOUCH BUMPER HERE
        forward_encoderMecanum(1000, -100, 0, Lf, Lb, Rf, Rb);
        forward_encoderMecanum(500, -25, 0, Lf, Lb, Rf, Rb);
        turnToDeg_Mecanum(45, 50, Lf, Lb, Rf, Rb);
        forward_encoderMecanum(500, 0, 75, Lf, Lb, Rf, Rb);
        move_encoderortouch(500, -25, 0, Lf, Lb, Rf, Rb);
    }
    if (zone == 1){
        servo[TouchSensor] = 190;
        //Nav to zone 1 (farthest)
        forward_encoderMecanum(2500, 0, 100, Lf, Lb, Rf, Rb);
        wait1Msec(250);
        forward_encoderMecanum(2300, -100, 0, Lf, Lb, Rf, Rb);
        forward_encoderMecanum(1100, -25, 0, Lf, Lb, Rf, Rb);
        turnToDeg_Mecanum(75, 50, Lf, Lb, Rf, Rb);
        wait1Msec(100);
        move_encoderortouch(1200, -100, 0, Lf, Lb, Rf, Rb);
        move_encoderortouch(500, -25, 0, Lf, Lb, Rf, Rb);
    }
    wait1Msec(500);
    if (!options_get[1] == 1){
      servo[BallStorage] = BallStorage_Open;
    }
    else{
      servo[BallStorage] = BallStorage_OpenSmall;
    }
    motor[Intake] = 0;
    servo[TouchSensor] = 65;
    wait1Msec(250);
    return zone;
}
void auto_centerGoalToLarge(float zone,float dir){
    bool moveleft = false;
    bool moveright = false;
    if (dir == 1){
        moveleft = true;
    }
    if (dir == 2){
        moveright = true;
    }
    servo[GoalRetainer] = 130;
    if (zone == 3){
        forward_encoderMecanum(667,15,0, Lf, Lb, Rf, Rb);
        if(!moveleft){
            forward_encoderMecanum(2667, 0, -100, Lf, Lb, Rf, Rb);
            wait1Msec(10);
            forward_encoderMecanum(6250, -50, 0, Lf, Lb, Rf, Rb);
            turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(3500, -25, 0, Lf, Lb, Rf, Rb);
            forward_Mecanum(1000,-50,0,Lf,Lb,Rf,Rb);
            servo[GoalRetainer] = GoalRetainer_Closed;
            forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
        }
        else{
            forward_encoderMecanum(3000, 0, 100, Lf, Lb, Rf, Rb);
            forward_encoderMecanum(6000, -50, 0, Lf, Lb, Rf, Rb);
            wait1Msec(250);
            turnToDeg_Mecanum(75,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(6300, -50, 0, Lf, Lb, Rf, Rb);
            turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(750, -25, 0, Lf, Lb, Rf, Rb);
            forward_Mecanum(1000,-25,0,Lf,Lb,Rf,Rb);
            servo[GoalRetainer] = GoalRetainer_Closed;
            wait1Msec(500);
            forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
        }
    }
    if (zone == 2){
        forward_encoderMecanum(500, 15, 0, Lf, Lb, Rf, Rb);
        if(!moveright){
            //turnToDeg_Mecanum(0,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(2133, 0, 100, Lf, Lb, Rf, Rb);
            forward_encoderMecanum(5273, -50, 0, Lf, Lb, Rf, Rb);
            wait1Msec(250);
            turnToDeg_Mecanum(75,50,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(5900, -50, 0, Lf, Lb, Rf, Rb);
            turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(750, -25, 0, Lf, Lb, Rf, Rb);
            forward_Mecanum(2000,-25,0,Lf,Lb,Rf,Rb);
            servo[GoalRetainer] = GoalRetainer_Closed;
            wait1Msec(500);
            forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
        }
        else{
            //Zone 2 right
            forward_encoderMecanum(4000, 0, -100, Lf, Lb, Rf, Rb);
            wait1Msec(10);
            forward_encoderMecanum(5250, -50, 0, Lf, Lb, Rf, Rb);
            turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
            forward_encoderMecanum(3500, -25, 0, Lf, Lb, Rf, Rb);
            forward_Mecanum(2000,-50,0,Lf,Lb,Rf,Rb);
            servo[GoalRetainer] = GoalRetainer_Closed;
            forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
        }
    }
    if (zone == 1){
        forward_encoderMecanum(333, 15, 0, Lf, Lb, Rf, Rb);
        turnToDeg_Mecanum(0,50,Lf,Lb,Rf,Rb);
        forward_encoderMecanum(3000, -50, 0, Lf, Lb, Rf, Rb);
        turnToDeg_Mecanum(75,50,Lf,Lb,Rf,Rb);
        forward_encoderMecanum(6150, -50, 0, Lf, Lb, Rf, Rb);
        turnToDeg_Mecanum(42,35,Lf,Lb,Rf,Rb);
        forward_encoderMecanum(600, -25, 0, Lf, Lb, Rf, Rb);
        forward_Mecanum(2000,-25,0,Lf,Lb,Rf,Rb);
        servo[GoalRetainer] = GoalRetainer_Closed;
        forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
    }
    wait1Msec(500);
    //Drop both balls if not moving to 90 cm
    servo[BallStorage] = BallStorage_Open;
    wait1Msec(500);
}
/**** PLACE IN KICKSTAND
(from already placing a ball in center goal) ****/
void auto_kickstandFromCenterGoal(int zone)
{
    //hook the goal
    wait1Msec(250);
    forward_encoderMecanum(1300, 0, 100, Lf, Lb, Rf, Rb);
    wait1Msec(250);
    forward_encoderMecanum(600, 100, 0, Lf, Lb, Rf, Rb);
    wait1Msec(250);
    forward_encoderMecanum(1000, 0, -100, Lf, Lb, Rf, Rb); //hit the goal
    wait1Msec(250);
    forward_encoderMecanum(2200, 100, 0, Lf, Lb, Rf, Rb); //continue moving
}
