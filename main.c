//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it

int powStrum=0;
string song = "";
char one, two, three, four;
const int MINTOMSEC = 60000;
#include "PC_FileIO.c"

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

void powerMotorStrum (int m1, int DEGREES, int POW, int RETURNPOW, int beat)
{
	clearTimer(T1);
    /*nMotorEncoder[m1]=0;
    motor[m1]=POW;
    while(nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=-RETURNPOW;
    while (nMotorEncoder[m1]>DEGREES)
    {}
    motor[m1]=0;*/
		powerMotor(m1,DEGREES,POW,RETURNPOW);
    while(time1[T1] < (beat/2))
    {}
}

//The RETURNPOW is to ensure it stops at the right place

//Input exact same parameters as powerMotor function but put a negative sign before degrees
//I can change this and have the negative sign in the function itself if that makes it more intuitive

void powerMotorBack (int m1, const int DEGREES, const int POW, const int RETURNPOW)
{
	clearTimer(T1);
    nMotorEncoder[m1]=0;
    motor[m1]=-POW;
    while(nMotorEncoder[m1]>DEGREES)//e.g -180
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=RETURNPOW;
    while (nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
}

void powerMotorBackStrum (int m1, const int DEGREES, const int POW, const int RETURNPOW, int beat)
{
	clearTimer(T1);
    /*nMotorEncoder[m1]=0;
    motor[m1]=-POW;//
    while(nMotorEncoder[m1]>DEGREES)//e.g -180
    {}
    motor[m1]=0;
    wait10Msec(5);
    motor[m1]=RETURNPOW; //returnpow should be negative and small/slow
    while (nMotorEncoder[m1]<DEGREES)
    {}
    motor[m1]=0;*/
	powerMotorBack(m1,DEGREES,POW,RETURNPOW);
    while(time1[T1] < beat/2)
    {}
}

void songChoice (int SColor, string&song)
{
    while (SensorValue[SColor]==(int)colorWhite) {}
    if (SensorValue[SColor]==(int)colorRed)
    {
        song = "\"Red.txt\"";
    }
    else if (SensorValue[SColor]==(int)colorGreen) //or another more distinct color
    {
        song="\"Green.txt\"";
    }
}

void waitUltra(int SUS)
{
	while(SensorValue(SUS)>=40)
	{}
	displayString(3, "Green coin is Riptide");
	displayString(5, "Red coin is I'm Yours");
	displayString(7, "Please insert a coin");
}

float bpmCalc(float bpm)
{
    return bpm/MINTOMSEC;
}

int readFile ()
{
    TFileHandle fin;
    float beat=0;
    bool fileCheck = openReadPC(fin,song);

    if(!fileCheck)
    {
        displayString(5, "Song cannot be found");
        wait1Msec(5000);
				return -1;
    }
    else
    {
        string songName = "";
        int bpm = 0;
        readTextPC(fin, songName);
        displayBigTextLine(2, "Up next: %s", songName);
        readIntPC(fin, bpm);
        beat = bpmCalc(bpm);

	readTextPC(fin, one);
	readTextPC(fin, two);
	readTextPC(fin, three);
	readTextPC(fin, four);
    }
    return beat;
}



void powerMotorPick(int motorA, int DEGREESPICK, int POWPICK, int RETURNPOW, bool & pick, bool & prevPick)
{
	if(pick == prevPick)
	{}
	else if(pick == false)
	{
		powerMotorBack(motorA, -DEGREESPICK, POWPICK, RETURNPOW);
	}
	else
	{
		powerMotor(motorA, DEGREESPICK, POWPICK, RETURNPOW);
	}

	prevPick = pick;
 }

int rotationChord (char & initialPosition, char newPosition)
{
	//this function returns the angle the chord box needs to turn
	//and also redefines newPosition to initialPosition in the end unless newPosition is 0
	//the first initialPosition is read in from the input file in main
	//and then after it's read in we run this function
	//order of sides: C then D then E then F (replace these with the actual chords)
	if(initialPosition==newPosition)
	{
		return 0;
	}
	else if (initialPosition==one && newPosition==two ||
		 initialPosition==two && newPosition==three ||
		 initialPosition==three && newPosition==four ||
		 initialPosition==four && newPosition==one)
		{
			return 90;
			initialPosition=newPosition;
		}

	else if (initialPosition==one && newPosition==three ||
		 initialPosition==two && newPosition==four ||
		 initialPosition==three && newPosition==one ||
		 initialPosition==four && newPosition==two)
		{
			return	180;
			initialPosition=newPosition;
		}
	else if (initialPosition==one && newPosition==four ||
		 initialPosition==two && newPosition==one ||
		 initialPosition==three && newPosition==two ||
		 initialPosition==four && newPosition==three)
		{
			return -90;
		 	initialPosition=newPosition;
		}
		else //newPosition=0 so initialPosition shouldn't change
		{
			return 0;
		}

}

void powerChord (int motorC, int motorD, int DEGREESPISTON, int POWPISTON,
								 int POWCHORD, int RETURNPOW, char initialPosition,
								 char newPosition)
{
	int rotation = rotationChord (initialPosition, newPosition);
	if (rotation!=0)
	{
		powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
		powerMotor(motorD, rotation, POWCHORD,RETURNPOW);
		powerMotorBack(motorC, -DEGREESPISTON, POWPISTON, RETURNPOW);
	}
}


task main()
{
    init(S1, S2, S3); //STouch=S1, SUS=S2, SColor=S3
    waitUltra(S2);

    //ReadFile(S3);
    //int beat = 0;
    //beat = ReadFile(S3);

    //Insert a read input file function
    //DEGREESSTRUM values might depend on song choice so if statement might be needed

    const int DEGREESSTRUM=55, DEGREESPICK=35, DEGREESCHORD=90, DEGREESPISTON=180;
    const int POWCHORD=100, POWPISTON=100, POWPICK=70;
    const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?
		bool pick = true, prevPick = true; //testing

		//songChoice (S3, song);
		//readFile();
		displayBigTextLine (6,"Press the start/stop button to play");

		while (SensorValue[S1]==0){}
		eraseDisplay();
		while (SensorValue[S1]==1){}
		displayBigTextLine(3, "WORK BITCH");
		//You insert the coin and then press the start button to start
	  /*displayBigTextLine(3, "Now Playing:");
		if (song=="Riptide_Chords.txt")
		{
			displayBigTextLine(6, "Riptide");
		}
		else
		{
			displayBigTextLine(6, "I'm Yours");
		}
	*/

		while (SensorValue[S1]==0) // ||file read in -1)
	    {
	    	eraseDisplay();
	    	displayBigTextLine(2, "Am i working");
	    	//wait1Msec(5000);
	    	//motorA: Pick Mechanism
	    	//motorB: Strumming Mechanism
	    	//motorC: Piston Mechanism
	    	//motorD: Rotation/Chord Mechanism
	    	//wait10Msec(5);

	    	nMotorEncoder[motorA]=0;
			float beat=bpmCalc(72);
			pick = false;
			//eraseDisplay();
			//displayString(2,".d%",beat);
			//wait1Msec(5000);
			powerMotorPick(motorA, DEGREESPICK, 100, RETURNPOW, pick, prevPick);
	    	powerMotorStrum(motorB, 45, 60, RETURNPOW, beat);
	    	pick = true;
			powerMotorPick(motorA, DEGREESPICK, 100, RETURNPOW, pick, prevPick);
			motor[motorA]=5;
	    	powerMotorBackStrum(motorB, -45, 60, RETURNPOW, beat);
	   	  	motor[motorA]=0;
	    	wait1Msec(5);

	 		time1[T1] = 0;
			/*	char initialPosition='C';
	    	char newPosition='D';
	    	powerChord (motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
	    	initialPosition, newPosition);
	    	*/

	    	//this is just testing stuff
				//float beat=bpmCalc(20,minToMSec);
				/*powerMotor(motorA, DEGREESPICK, POWPICK, RETURNPOW);
	    	powerMotorStrum(motorB, DEGREESSTRUM, 25, RETURNPOW, beat);
	    	powerMotorBackStrum(motorB, -DEGREESSTRUM, 25, RETURNPOW, beat);
	    	*/

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
	    }
	    eraseDisplay();
	    displayBigTextLine(4, "Program ended");
	    wait1Msec(3000);
}
