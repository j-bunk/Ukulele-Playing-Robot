//Work in RobotC then copy and paste your code here and push to Github
//If your working on a specific function you can create a branch work on it then merge it

#include "EV3_FileIO.c"

void init(int STouch, int SUS, int SGyro,int SColor)
{
    SensorType[STouch]=sensorEV3_Touch;
    SensorType[SUS]=sensorEV3_Ultrasonic;
    //SensorType[SGyro]=sensorEV3_Gyro;
    //SensorMode[SGyro]=modeEV3Gyro_RateAndAngle;
    //while (getGyroRate(SGyro) != 0) {}
    //resetGyro(SGyro);
    wait1Msec(50);
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
    wait1Msec(50);
    motor[m1]=-RETURNPOW;
    while (nMotorEncoder[m1]>DEGREES)
    {}
    motor[m1]=0;
    wait10Msec(5);
}

void powerMotorStrum (int m1, int motorA, int DEGREES, int POW, int RETURNPOW, int beat, string newPosition)
{
	clearTimer(T1);
			   	if (newPosition!="0")
			    {
					    motor[motorA]=5;
			  	}
		powerMotor(m1,DEGREES,POW,RETURNPOW);
    while(time1[T1] < (beat/2))
    {}
		motor[motorA] = 0;
		wait1Msec(50);
}

//The RETURNPOW is to ensure it stops at the right place

//Input exact same parameters as powerMotor function but put a negative sign before degrees
//I can change this and have the negative sign in the function itself if that makes it more intuitive
void powerMotorBack (int m1, int DEGREES, int POW, int RETURNPOW)
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

void powerMotorBackStrum (int m1, int DEGREES, int POW, int RETURNPOW, int beat, string newPosition)
{
    clearTimer(T1);
		if (newPosition!="0")
		{
				motor[motorA]=5;
		}
		powerMotorBack(m1,DEGREES,POW,RETURNPOW);
    while(time1[T1] < beat/2)
    {}
  	motor[motorA]=0;
  	wait1Msec(50);
}

void waitUltra(int SUS)
{
	while(SensorValue(SUS)>=80)
	{}
	displayString(5, "Hello!");
	displayString(7, "Press the left button to"); 
	displayString(8, "play Riptide and the right");
	displayString(9, "button to play I'm Yours");
}

void songChoice (string&songFile, string RED, string BLUE) 
{  
	while (!getButtonPress(buttonRight)&& !getButtonPress(buttonLeft)){}
	if (getButtonPress(buttonLeft))
	{
		songFile = RED;
  }
	else
	{
		songFile=BLUE;
	}
}
		/*wait1Msec(50);
		while(!getGyroDegrees(sGyro)>35 || !getGyroDegrees(sGyro)>-35){}
		if (getGyroDegrees(sGyro)>35)
		{
		songFile = RED;
    displayString(9, "Red");
    wait1Msec(2000);
  	}
  	else if (getGyroDegrees(sGyro)<-35)  
  	{
  			songFile=BLUE;
        displayString(9, "Blue");
        wait1Msec(2000);
     }
     else
     {
       displayString(10, "Error");
       wait1Msec(2000);
     }
  		
    /*while (SensorValue[S3]==6) //white is 6 and red is 5
    {
    	displayString(9, "Stuck in white");
    }
    while (SensorValue[S3]!=5)// && SensorValue[S3] != 2)
    {
    	displayString(9, "Stuck in decision");
    }*/
    //if (SensorValue[S3]==5)
    //{
      
    //}
    /*else if (SensorValue[S3] == 2) //(SensorValue[SColor]==(int)color) //or another more distinct color
    {
        songFile=BLUE;
        displayString(9, "Blue");
        wait1Msec(2000);
    }
    else
    {
    	displayString(2, "Colour detection failed. ");
    	wait1Msec(2000);
    }*/


void readFileInit (TFileHandle&fin, float&beat, string&songName, int MINTOMSEC, string&one, string&two, string&three, string&four)
{
    int bpm = 1;
    eraseDisplay();
    readTextPC(fin, songName);
    displayString(2, "Up next: %s", songName);
    readIntPC(fin, bpm);
    beat = MINTOMSEC/bpm;
		readTextPC(fin, one);
		readTextPC(fin, two);
		readTextPC(fin, three);
		readTextPC(fin, four);
}

void powerMotorPick(int motorA, int DEGREESPICK, int POWPICK, int RETURNPOW, string newPosition, string&prevPick)
{
	if((newPosition == prevPick) || (newPosition !="0" && prevPick!="0"))
	{}
	else if(newPosition == "0")
	{
		powerMotorBack(motorA, -DEGREESPICK, POWPICK, RETURNPOW);
	}
	else
	{
		powerMotor(motorA, DEGREESPICK, POWPICK, RETURNPOW);
	}
	prevPick = newPosition;
	wait1Msec(50);
}

int rotationChord (string & initialPosition, string newPosition, string one, string two, string three, string four)
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
		initialPosition=newPosition;
		return 90;
	}

	else if (initialPosition==one && newPosition==three ||
					 initialPosition==two && newPosition==four ||
		 			 initialPosition==three && newPosition==one ||
		 			 initialPosition==four && newPosition==two)
	{
		initialPosition=newPosition;
		return	180;
	}
	else if (initialPosition==one && newPosition==four ||
		 			initialPosition==two && newPosition==one ||
		 			 initialPosition==three && newPosition==two ||
		   		 initialPosition==four && newPosition==three)
	{
		initialPosition=newPosition;
		return 270;
	}
	else //newPosition=0 so initialPosition shouldn't change
	{
		return 0;
	}
}

void powerChord (int motorC, int motorD, int DEGREESPISTON, int POWPISTON,
								 int POWCHORD, int RETURNPOW, string &initialPosition,
								 string newPosition, string one, string two, string three, string four)
{
	int rotation = rotationChord (initialPosition, newPosition, one, two, three, four);
	if (rotation!=0)
	{
		powerMotor(motorC, DEGREESPISTON, POWPISTON, RETURNPOW);
		powerMotor(motorD, rotation, POWCHORD,RETURNPOW);
		powerMotorBack(motorC, -DEGREESPISTON, POWPISTON, RETURNPOW);
	}
}


task main()
{
	string songFile = "", one="", two="", three="", four="", songName = "";
	const int MINTOMSEC = 60000;
	string RED = "Red.txt", BLUE = "Blue.txt";
	float beat = 0;
	TFileHandle fin;
  init(S1, S2, S3, S4); //STouch=S1, SUS=S2, SGyro=S3, SColor=S4
  waitUltra(S2);
	songChoice (songFile, RED, BLUE);
	openReadPC(fin, songFile);
	 /*bool fileCheck = openReadPC(fin,songName);

   if(!fileCheck)
   {
       displayString(5, "Song cannot be found");
       wait1Msec(5000);
   }*/
   readFileInit (fin, beat, songName, MINTOMSEC, one, two, three, four);

   //Insert a read input file function
   //DEGREESSTRUM values might depend on song choice so if statement might be needed

   const int DEGREESSTRUM=40, DEGREESPICK=35, DEGREESPISTON=180;
   const int POWCHORD=50, POWPISTON=50, POWPICK=75, POWSTRUM=60;
   const int RETURNPOW=10; //Should each mechanism have different RETURNPOW values?

		displayString (6,"Press the start/stop button");
		displayString (7,"to play");

		while (SensorValue[S1]==0){}
		while (SensorValue[S1]==1){}

		eraseDisplay();
	  displayBigTextLine(3, "Now Playing:");
	  displayBigTextLine(6, "%s", songName);

		string newPosition = "0";
		string initialPosition=one;
		string prevPick="start";
		powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, prevPick);
		wait1Msec(1000);
		powerMotorStrum(motorB, motorA, DEGREESSTRUM, POWSTRUM, RETURNPOW, beat, newPosition);
		wait1Msec(1000);
		powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, initialPosition, prevPick);
		wait1Msec(1000);
		while (SensorValue[S1]==0 && newPosition!="-1")
	    {
		    	//motorA: Pick Mechanism
		    	//motorB: Strumming Mechanism
		    	//motorC: Piston Mechanism
		    	//motorD: Rotation/Chord Mechanism
			    readTextPC(fin, newPosition);
			    powerChord(motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
			    					 initialPosition, newPosition, one, two, three, four);
					powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, prevPick);
					wait1Msec(50);
			   	powerMotorBackStrum(motorB, -DEGREESSTRUM, POWSTRUM, RETURNPOW, beat, newPosition);
			   	readTextPC(fin,newPosition);
			   	powerChord(motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW,
			    					 initialPosition, newPosition, one, two, three, four);
			    powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, prevPick);
			    wait1Msec(50);
			    powerMotorStrum(motorB, motorA, DEGREESSTRUM, POWSTRUM, RETURNPOW, beat, newPosition);
	    }
	    newPosition = one;
			powerChord(motorC, motorD, DEGREESPISTON, POWPISTON, POWCHORD, RETURNPOW, initialPosition, newPosition, one, two, three, four,);
			powerMotorBackStrum(motorB, -DEGREESSTRUM, POWSTRUM, RETURNPOW, beat, newPosition);
			powerMotorPick(motorA, DEGREESPICK, POWPICK, RETURNPOW, newPosition, prevPick);

	    eraseDisplay();
	    displayBigTextLine(4, "Program ended");
	    wait1Msec(3000);
}
