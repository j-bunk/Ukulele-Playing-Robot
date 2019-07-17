//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it
//Initialization function
int POWSTRUM=0;
void init(int STouch, int SUS, int SColor)
{
    SensorType[STouch]=sensorEV3_Touch;
    //ultrasonic and color too?
    SensorType[SUS]=sensorEV3_Ultrasonic;
    SensorType[SColor]=sensorEV3_Color;
    SensorMode[SColor]=modeEV3Color_Color;
}

void powerMotor (int m1, int DEGREES, int POW, int RETURNPOW)
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
//The RETURNPOW is to ensure it stops at the right place

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

int songChoice (int SColor)
{
    //red is 5 and blue is 2
    while (SensorValue[SColor]==ColorBlackValue) //don't know the value and black is the color of the back of the coin box
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

void ReadFile (int SColor)
{
    int choice= songChoice (SColor);
    if (choice==1)
    {
        //Read song 1
        POWSTRUM=20;
    }
    else if (choice==2)
    {
        //Read song 2
        POWSTRUM=40;
    }
}

task main()
{
    init(S1, S2, S3);
    //US function
    ReadFile(S3);
    //Prompt song choice display after someone is within a certain distance
        //Insert Ultrasonic function / prompt song choice function
        //Insert Song choice function
        //Insert a read inpput file function
        //Does the song choice determine the values of the constant?

        //DEGREESSTRUM values might depend on song choice so if statement might be needed
        const int DEGREESSTRUM=55, DEGREESPICK=90; DEGREESCHORD=90, DEGREESPISTON=180;
        const int POWCHORD=60, POWPISTON=40, POWPICK=20;
        const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?
        POWSTRUM=0;

    while (SensorValue[S1]==0 || //file read in -1)
    {
        //bunch of if statements that call these functions based on the input file
        powerMotor(motorA, DEGREESSTRUM, POWSTRUM, RETURNPOW);
        powerMotor(motorB, DEGREESCHORD, POWCHORD, RETURNPOW);
        powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
        powerMotor(motorD, DEGREESPICK, POWPICK, RETURNPOW);
    }

    displayBigTextLine(4, "Program ended");
    wait1Msec(3000);
}

//edit
