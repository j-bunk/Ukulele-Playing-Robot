//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it

int powStrum=0;
string song = "";
float minToMSec = 60000;
void init(int STouch, int SUS, int SColor)
{
    SensorType[STouch]=sensorEV3_Touch;
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
    wait10Msec(5);
    motor[m1]=-RETURNPOW;
    while (nMotorEncoder[m1]>DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
}

void powerMotorStrum (int m1, int DEGREES, int POW, int RETURNPOW, float beat)
{
		clearTimer(T1);
    nMotorEncoder[m1]=0;
    motor[m1]=POW;
    while(nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=-RETURNPOW;
    while (nMotorEncoder[m1]>DEGREES)
    {}
    motor[m1]=0;
    while(time1[T1] < (beat/2))
    {}
}
//The RETURNPOW is to ensure it stops at the right place

void powerMotorBack (int m1, const int DEGREES, const int POW, const int RETURNPOW)
{
		clearTimer(T1);
    nMotorEncoder[m1]=0;
    motor[m1]=-POW;//
    while(nMotorEncoder[m1]>DEGREES)//e.g -180
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=RETURNPOW; //returnpow should be negative and small/slow
    while (nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
}
void powerMotorBackStrum (int m1, const int DEGREES, const int POW, const int RETURNPOW, float beat)
{
		clearTimer(T1);
    nMotorEncoder[m1]=0;
    motor[m1]=-POW;//
    while(nMotorEncoder[m1]>DEGREES)//e.g -180
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=RETURNPOW; //returnpow should be negative and small/slow
    while (nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    while(time1[T1] < beat/2)
    {}
}

/*void songChoice (int SColor, string&song)
{
    //red is 5 and blue is 2
    while (SensorValue[SColor]==(int)colorBlack) //don't know the value and black is the color of the back of the coin box
    {}
    if (SensorValue[SColor]==5)
    {
        song = "Song1.c";
    }
    else if (SensorValue[SColor]==2)//or another more distinct color
    {
        song="Song2.c";
    }
}

/*#include "Song1.c";
#include "Song2.c";

float readFile (int SColor)
{
    songChoice(SColor, song);
    TFileHandle fin;

    bool fileCheck = openReadPC(fin,song);
=======
    int beat = 0;
    string songFile = songChoice(SColor);
    TfileHandle fin;

    bool fileCheck = openReadPC(fin,songFile);
>>>>>>> 676f04dadfa0aa0eb775e0f0de177e67467eef79
    if(!fileCheck)
    {
        displayString(5, "Song cannot be found");
        wait1Msec(5000);
    }
    else
    {
        string songName = "";
        int bpm = 0;
        readTextPC(fin, songName);
        displayString(5, "Now Playing: %s", songName);
        //wait here? or will there be other things that need to display?
        readIntPC(fin, bpm);
        beat = bpmCalc(bpm, minToMSec);
        //powStrum = bpm conversion function
    }
    return beat;
}
*/
int bpmCalc(float bpm, float minToMSec)
{
    float beat = 0;
    beat = minToMSec / bpm;
    return beat;
}

int rotationChord (char & initialPosition, char newPosition)
{
	//the first initialPosition is read in from the input file in main
	//and then after it's read in we run this function
	//order of sides: C then D then E then F
	if(initialPosition==newPosition)
	{
		return 0;
	}
	else if (initialPosition=='C' && newPosition=='D' ||
					 initialPosition=='D' && newPosition =='E' ||
					 initialPosition=='E' && newPosition=='F' ||
					 initialPosition=='F' && newPosition=='C')
		{
			return 90;
		}

	else if (initialPosition=='C' && newPosition=='E' ||
					 initialPosition=='D' && newPosition =='F' ||
					 initialPosition=='E' && newPosition=='C' ||
					 initialPosition=='F' && newPosition=='D')
		{
			return	180;
		}
	else if (initialPosition=='C' && newPosition=='F' ||
					 initialPosition=='D' && newPosition =='C' ||
					 initialPosition=='E' && newPosition=='D' ||
					 initialPosition=='F' && newPosition=='E')
		{
		  return 270;//or -90?
		}
		else
		{
			return 0;
		}
		initialPosition=newPosition;
}

void powerChord (int motorD, int DEGREESPISTON, int POWPISTON,
int POWCHORD, int RETURNPOW, char initialPosition, char newPosition)
{
	int rotation = rotationChord (initialPosition, newPosition);
	powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
	powerMotor(motorD, rotation, POWCHORD,RETURNPOW);
	powerMotorBack(motorC, -DEGREESPISTON, POWPISTON, RETURNPOW);

	//do I include an if statement that doesn't move it when newposition=initialposition
}


task main()
{
    init(S1, S2, S3);
    //US function
    //ReadFile(S3);
    //int beat = 0;
    //beat = ReadFile(S3);
    //Prompt song choice display after someone is within a certain distance
        //Insert Ultrasonic function / prompt song choice function
        //Insert Song choice function
        //Insert a read inpput file function
        //Does the song choice determine the values of the constant?

        //DEGREESSTRUM values might depend on song choice so if statement might be needed
        const int DEGREESSTRUM=55, DEGREESPICK=10, DEGREESCHORD=90, DEGREESPISTON=180;
        const int POWCHORD=60, POWPISTON=40, POWPICK=40;
        const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?

    while (SensorValue[S1]==0) // ||file read in -1)
    {
			/*float beat=bpmCalc(20,minToMSec);
			powerMotor(motorA, DEGREESPICK, POWPICK, RETURNPOW)
    	powerMotorStrum(motorB, DEGREESSTRUM, 25, RETURNPOW, beat);
    	powerMotorBackStrum(motorB, -DEGREESSTRUM, 25, RETURNPOW, beat);
    	*/
    	char initialPosition='C';
    	char newPosition='D';
    	powerChord (motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
    	initialPosition, newPosition)


        //bunch of if statements that call these functions based on the input file
       /* powerMotor(motorA, DEGREESSTRUM, powStrum, RETURNPOW);
        powerMotorBack(motorA, DEGREESSTRUM, powStrum, RETURNPOW);
        powerMotor(motorB, DEGREESCHORD, POWCHORD, RETURNPOW);
        powerMotorBack(motorB, DEGREESCHORD, POWCHORD, RETURNPOW);
        powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
        powerMotorBack(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
        powerMotor(motorD, DEGREESPICK, POWPICK, RETURNPOW);
        powerMotorBack(motorD, DEGREESPICK, POWPICK, RETURNPOW);
        */

        time1[T1] = 0;
    }
    displayBigTextLine(4, "Program ended");
    wait1Msec(3000);
}

//edit
