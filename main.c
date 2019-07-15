//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it
//Initialization function

void init(string STouch, string SUS, string SColor)
{
    //Are the first 2 lines necessary?
    motor[motorA]=motor[motorB]=motor[motorC]=0;
    nMotorEncoder[motorA]=nMotorEncoder[motorB]=nMotorEncoder[motorC]=nMotorEncoder[motorD]=0;
    SensorType[STouch]=sensorEV3_Touch;
    //ultrasonic and color too?
    SensorType[SUS]=sensorEV3_Ultrasonic;
    SensorType[SColor]=sensorEV3_Color;
}

void powerMotor (string m1, const int DEGREES, const int POW, const int RETURNPOW)
{
    nMotorEncoder[m1]=0;
    motor[m1]=POW;
    while(nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(50);
    motor[m1]=-RETURNPOW; //returnpow should be negative and small/slow
    while (nMotorEncoder[m1]>DEGREES)
    {}
    motor[m1]=0;
}

void powerMotorBack (string m1, const int DEGREES, const int POW, const int RETURNPOW)
{
    nMotorEncoder[m1]=0;
    motor[m1]=-POW;//
    while(nMotorEncoder[m1]>DEGREES)//e.g -180
    {}
    motor[m1]=0;
    wait10Msec(50);
    motor[m1]=RETURNPOW; //returnpow should be negative and small/slow
    while (nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
}

int songChoice (string SColor)
{
    //red is 5, blue is 2
    while (SensorValue[SColor]==ColorBlackValue) //don't know the value and black is the color of the back of the box
    {}
    if (SensorValue[SColor]==5)
    {
        return 1;
    }
    else if (SensorValue[SColor]==2)//or another more distinct color
    {
        return 2;
    }
}

void ReadFile (SColor)
{
    int choice=songChoice ();
    if (choice==1)
    {
        //Read song 1
    }
    else if (choice==2)
    {
        //Read song 2
    }
}

task main()
{
    init(S1, S2, S3);
    //Prompt song choice display after someone is within a certain distance
        //Insert Ultrasonic function / prompt song choice function
        //Insert Song choice function
        //Insert a read inpput file function
        //Does the song choice determine the values of the constant?

        //DEGREESSTRUM values might depend on song choice so if statement might be needed
        const int DEGREESSTRUM=55, DEGREESCHORD=90, DEGREESPISTON=180;
        const int POWSTRUM=50, POWCHORD=60, POWPISTON=40;
        const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?

    while (SensorValue[S1]==0)
    {   
        //bunch of if statements that call these functions based on the input file
        powerMotor(motorA, DEGREESSTRUM, POWSTRUM, RETURNPOW);
        powerMotor(motorB, DEGREESCHORD, POWCHORD, RETURNPOW);
        powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
    }
    displayBigTextLine(4, "Program ended");
    wait1Msec(3000);
}