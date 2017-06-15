#include "SproutLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

SproutLightPatterner::SproutLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	seedArray[0]  = singleMan->lightMan()->channelArray[0];
	hubArray[0]   = singleMan->lightMan()->channelArray[1];
	petalArray[0] = singleMan->lightMan()->channelArray[2];
	petalArray[1] = singleMan->lightMan()->channelArray[3];
	stemArray[0]  = singleMan->lightMan()->channelArray[4];

	seedArray[1]  = singleMan->lightMan()->channelArray[5];
	hubArray[1]   = singleMan->lightMan()->channelArray[6];
	petalArray[2] = singleMan->lightMan()->channelArray[7];
	petalArray[3] = singleMan->lightMan()->channelArray[8];
	stemArray[1]  = singleMan->lightMan()->channelArray[9];

	seedArray[2]  = singleMan->lightMan()->channelArray[10];
	hubArray[2]   = singleMan->lightMan()->channelArray[11];
	petalArray[4] = singleMan->lightMan()->channelArray[12];
	petalArray[5] = singleMan->lightMan()->channelArray[13];
	stemArray[2]  = singleMan->lightMan()->channelArray[14];

	seedArray[3]  = singleMan->lightMan()->channelArray[15];
	hubArray[3]   = singleMan->lightMan()->channelArray[16];
	petalArray[6] = singleMan->lightMan()->channelArray[17];
	petalArray[7] = singleMan->lightMan()->channelArray[18];
	stemArray[3]  = singleMan->lightMan()->channelArray[19];

	seedArray[4]  = singleMan->lightMan()->channelArray[20];
	hubArray[4]   = singleMan->lightMan()->channelArray[21];
	petalArray[8] = singleMan->lightMan()->channelArray[22];
	petalArray[9] = singleMan->lightMan()->channelArray[23];
	stemArray[4]  = singleMan->lightMan()->channelArray[24];
}

void SproutLightPatterner::drawPattern() {
	static unsigned long nextSwitch = 0;
	static int currPattern = -1;
	static byte patternVar = 0;

	if(nextSwitch <= millis()) {
		currPattern++;
		if(currPattern >= 3)
			currPattern = 0;

		patternVar = random(256);

		nextSwitch = millis() + choosePattern(currPattern, patternVar);
	}	else {
		choosePattern(currPattern, patternVar);
	}
}

unsigned long SproutLightPatterner::choosePattern(int currPattern, byte patternVar)
{
	unsigned long nextLength = 0;
	if(currPattern == 0)
		nextLength = pattern_00(patternVar);
	else if(currPattern == 1)
		nextLength = pattern_01(patternVar);
	else if(currPattern == 2)
		nextLength = pattern_02(patternVar);

	return nextLength;
}

unsigned long SproutLightPatterner::pattern_00(byte solidColour) {
	colorWheelSnippetFade(seedArray,   5,   1, 8,  solidColour, solidColour);
	colorWheelSnippetFade(hubArray,    5,   1, 8,  solidColour, solidColour);
	colorWheelSnippetFade(petalArray, 10,   1, 8,  solidColour, solidColour);
	colorWheelSnippetFade(stemArray,   5,   1, 8,  solidColour, solidColour);
	return 1000*5; // 5 sec
}
unsigned long SproutLightPatterner::pattern_01(byte unused) {
	colorWheelSnippetFade(seedArray,   5,   4, 5,   60, 100);
	colorWheelSnippetFade(hubArray,    5,   6, 0,    0, 255,   true);
	colorWheelSnippetFade(petalArray, 10,   2, 10,  125, 235);
	colorWheelSnippetFade(stemArray,   5,   8, 20,    0,  40);
	return 1000*20; // 20 sec
}
unsigned long SproutLightPatterner::pattern_02(byte unused) {

	for(int i=0; i<25; i++) {
		singleMan->lightMan()->setColorToChannelFromWheelPosition(singleMan->lightMan()->channelArray[i], random(255));
	}
	delay(random(100,300));
	return 1000*8; // 10 sec
}

void SproutLightPatterner::colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize,
		int patternSpeedModifier, int brightnessSpeedModifier,
		byte wheelBegin, byte wheelEnd, bool wrap /*false*/) {

	unsigned long currTime = millis();

	/*	brightnessFade 40%->100%   4095 == off  1024 == 25%  2048 == 50%   0 == 100%     */
	/*   fade from 512->4095  (NOTE: to get less steps multiply brightness before use)  */
	int brightness = 0;
	if(brightnessSpeedModifier > 0) {
			/*		cos(rad)		Calculates the cos of an angle (in radians). The result will be between -1 and 1.
			cos(0) == 1
			cos(3.14) == -1
			cos(6.28) == 1
			*/
		long timeBetweenBrightnesses = brightnessSpeedModifier;
		int totalBrightSteps = 628;
		int brightnessAngleIndex = (currTime%(totalBrightSteps*timeBetweenBrightnesses))/timeBetweenBrightnesses;
		float cosBright = cos(brightnessAngleIndex/100.0);
		brightness = (cosBright + 1.0)*1800.0; // never turn it all the way off
	}

	int stepRange = wheelEnd-wheelBegin;
	int patternIndex = 0;
	int totalSteps = 0;
	if(stepRange > 0) {
		long dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
		long colorTimeBetweenSteps = patternSpeedModifier * dialSpeed;
		totalSteps = stepRange;
		if(wrap == false)
			totalSteps = stepRange*2;

		patternIndex = (currTime%(totalSteps*colorTimeBetweenSteps))/colorTimeBetweenSteps;
	}

	for(int i=0; i<arraySize; i++) {

		byte wheelStepPosition = 0;
		if(stepRange > 0) {
			int offset = totalSteps / arraySize;
			int currIndex = patternIndex + offset*i;
			if(currIndex > totalSteps)
				currIndex -= totalSteps;

			wheelStepPosition = currIndex<=stepRange ? currIndex : (stepRange - (currIndex-stepRange));
		}

		singleMan->lightMan()->setColorToChannelFromWheelPosition(colorArray[i], wheelBegin+wheelStepPosition, brightness);
	}
}
