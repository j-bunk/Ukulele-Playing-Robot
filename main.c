//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it

string song = "";
string one=" ", two=" ", three=" ", four=" ";
const int MINTOMSEC = 60000;
//#include "PC_FileIO.c"
#include "EV3_FileIO.c"

//#include "Green.txt"
//#include "Red.txt"

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

void waitUltra(int SUS)
{
	while(SensorValue(SUS)>=40)
	{}
	displayString(3, "Green card is Riptide");
	displayString(5, "Red card is I'm Yours");
	displayString(7, "Please insert a card");
}

float bpmCalc(float bpm)
{
    return MINTOMSEC/bpm;
}

void songChoice (string&song) //white is 6 and red is 5
{
    while (SensorValue[S3]==6) 
    {
    	displayString(9, "Stuck in white");
    }
    while (SensorValue[S3]!=5)
    {
    	displayString(9, "Stuck in decision");
    }
    if (SensorValue[S3]==5)
    {
        song = "Red.txt";
        displayString(9, "Red");
    }
    else if (SensorValue[S3] == 1) //(SensorValue[SColor]==(int)color) //or another more distinct color
    {
        song="Green.txt";
        displayString(9, "Green");
    }
    else
    {
    	displayString(2, "Colour detection failed. ");
    }
}

void readFileInit (TFileHandle&fin, float&beat)
{
    string songName = "abc";
    int bpm = 1;
    readTextPC(fin, songName);
    eraseDisplay();
    displayString(2, "Up next: %s", songName);
    readIntPC(fin, bpm);
    beat = bpmCalc(bpm);
    displayString(8, "%d", beat);	
		readTextPC(fin, one);
		readTextPC(fin, two);
		readTextPC(fin, three);
		readTextPC(fin, four);
}

void powerMotorPick(int motorA, int DEGREESPICK, int POWPICK, int RETURNPOW, string newPosition, string initialPosition)
{
	if(newPosition == initialPosition)
	{}
	else if(newPosition == "0")
	{
		powerMotorBack(motorA, -DEGREESPICK, POWPICK, RETURNPOW);
	}
	else
	{
		powerMotor(motorA, DEGREESPICK, POWPICK, RETURNPOW);
	}
}

int rotationChord (string & initialPosition, string newPosition)
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
		return 270;
		initialPosition=newPosition;
	}
	else //newPosition=0 so initialPosition shouldn't change
	{
		return 0;
	}
}

void powerChord (int motorC, int motorD, int DEGREESPISTON, int POWPISTON,
								 int POWCHORD, int RETURNPOW, string initialPosition,
								 string newPosition)
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

    const int DEGREESSTRUM=45, DEGREESPICK=35, DEGREESPISTON=180;
    const int POWCHORD=100, POWPISTON=100, POWPICK=100, POWSTRUM=60;
    const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?
		float beat = 0;
		TFileHandle fin;
    bool fileCheck = openReadPC(fin,song);

    if(!fileCheck)
    {
        displayString(5, "Song cannot be found");
        wait1Msec(5000);
    }
    
		songChoice (song);
		readFileInit(fin, beat);
		displayString (6,"Press the start/stop button to play");

		while (SensorValue[S1]==0){}
		while (SensorValue[S1]==1){}
		//You insert the card and then press the start button to start
		//eraseDisplay();
	  //displayBigTextLine(3, "Now Playing: %s", songName);
		/*if (song=="Riptide_Chords.txt")
		{
			displayBigTextLine(6, "Riptide");
		}
		else
		{
			displayBigTextLine(6, "I'm Yours");
		}
	*/
		string newPosition = "";
		string initialPosition=one;
		while (SensorValue[S1]==0 || newPosition=="-1")
	    {
	    	//motorA: Pick Mechanism
	    	//motorB: Strumming Mechanism
	    	//motorC: Piston Mechanism
	    	//motorD: Rotation/Chord Mechanism
	    
	    nMotorEncoder[motorA]=0;
	    readTextPC(fin,newPosition);
	    powerChord(motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
	    					 initialPosition, newPosition);
	   	powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, initialPosition);
	   	powerMotorStrum(motorB, DEGREESSTRUM, POWSTRUM, RETURNPOW, beat);
	   	time1[T1] = 0;
	   	readTextPC(fin,newPosition);
	   	powerChord(motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
	    					 initialPosition, newPosition);
	    powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, initialPosition);
	    powerMotorBackStrum(motorB, -DEGREESSTRUM, POWSTRUM, RETURNPOW, beat);
	    }
	    eraseDisplay();
	    displayBigTextLine(4, "Program ended");
	    wait1Msec(3000);
}
