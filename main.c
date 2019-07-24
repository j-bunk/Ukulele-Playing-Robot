//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it

int powStrum=0;
string song = "";
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
	powMotor(m1,DEGREES,POW,RETURNPOW);
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

int bpmCalc(int bpm)
{
    int beat = 0;
    beat = MINTOMSEC/ bpm;
    return beat;
}

int readFile ()
{
    TFileHandle fin;
	int beat = 0;
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
	else if (initialPosition=='1' && newPosition=='2' ||
		 initialPosition=='2' && newPosition=='3' ||
		 initialPosition=='3' && newPosition=='4' ||
		 initialPosition=='4' && newPosition=='1')
		{
			return 90;
			initialPosition=newPosition;
		}

	else if (initialPosition=='1' && newPosition=='3' ||
		 initialPosition=='2' && newPosition=='4' ||
		 initialPosition=='3' && newPosition=='1' ||
		 initialPosition=='4' && newPosition=='2')
		{
			return	180;
			initialPosition=newPosition;
		}
	else if (initialPosition=='1' && newPosition=='4' ||
		 initialPosition=='2' && newPosition=='1' ||
		 initialPosition=='3' && newPosition=='2' ||
		 initialPosition=='4' && newPosition=='3')
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

      const int DEGREESSTRUM=55, DEGREESPICK=35, DEGREESCHORD=90, DEGREESPISTON=180;
      const int POWCHORD=100, POWPISTON=100, POWPICK=70;
      const int RETURNPOW=10; 
	bool pick = true, prevPick = true; 

	songChoice (S3, song);
	readFile();
	displayBigTextLine (6,"Press the start/stop button to play");

	while (SensorValue[S1]==0){}
	while (SensorValue[S1]==1){}
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
    	//motorA: Pick Mechanism
    	//motorB: Strumming Mechanism
    	//motorC: Piston Mechanism
    	//motorD: Rotation/Chord Mechanism

	powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, pick, prevPick);
	powerMotor(motorB, DEGREESSTRUM, POWSTRUM, RETURNPOW);
	powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
	powerMotor(motorD, DEGREESCHORD, POWCHORD, RETURNPOW);
	powerMotorBack(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
	powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, pick, prevPick);
	powerMotorBack(motorB, DEGREESSTRUM, POWSTRUM, RETURNPOW);
	powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
	powerMotor(motorD, DEGREESCHORD, POWCHORD, RETURNPOW);
	powerMotorBack(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
    }
    eraseDisplay();
    displayBigTextLine(4, "Program ended");
    wait1Msec(3000);
}
