#include "SproutLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

SproutLightPatterner::SproutLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	seedArray[0] =  singleMan->lightMan()->channelArray[0];
	seedArray[1] =  singleMan->lightMan()->channelArray[1];
	seedArray[2] =  singleMan->lightMan()->channelArray[2];
	seedArray[3] =  singleMan->lightMan()->channelArray[3];
	seedArray[4] =  singleMan->lightMan()->channelArray[4];

	hubArray[0] =   singleMan->lightMan()->channelArray[5];
	hubArray[1] =   singleMan->lightMan()->channelArray[6];
	hubArray[2] =   singleMan->lightMan()->channelArray[7];
	hubArray[3] =   singleMan->lightMan()->channelArray[8];
	hubArray[4] =   singleMan->lightMan()->channelArray[9];

	petalArray[0] = singleMan->lightMan()->channelArray[10];
	petalArray[1] = singleMan->lightMan()->channelArray[11];
	petalArray[2] = singleMan->lightMan()->channelArray[12];
	petalArray[3] = singleMan->lightMan()->channelArray[13];
	petalArray[4] = singleMan->lightMan()->channelArray[14];
	petalArray[5] = singleMan->lightMan()->channelArray[15];
	petalArray[6] = singleMan->lightMan()->channelArray[16];
	petalArray[7] = singleMan->lightMan()->channelArray[17];
	petalArray[8] = singleMan->lightMan()->channelArray[18];
	petalArray[9] = singleMan->lightMan()->channelArray[19];

	stemArray[0] =  singleMan->lightMan()->channelArray[20];
	stemArray[1] =  singleMan->lightMan()->channelArray[21];
	stemArray[2] =  singleMan->lightMan()->channelArray[22];
	stemArray[3] =  singleMan->lightMan()->channelArray[23];
	stemArray[4] =  singleMan->lightMan()->channelArray[24];
}

void SproutLightPatterner::drawPattern() {
	drawSeed();
	drawHub();
	drawPetal();
	drawStem();
}



void SproutLightPatterner::drawSeed() {
//	chaseColor(seedArray, 5, 3, 1, 0, 0);
//	dialedColor(seedArray, 5, true);
	colorWheelSnippetFade(seedArray, 5,     2, 1,     60, 100);
}

void SproutLightPatterner::drawHub() {
//	chaseColor(hubArray, 5, 6, 1, 0, 1);
//	dialedColor(hubArray, 5, false);
	colorWheelSnippetFade(hubArray, 5,      3, 0,      0, 255,   true);
}

void SproutLightPatterner::drawPetal() {
//	chaseColor(petalArray, 10, 1, 0.0, 0.0, 1.0);
//	dialedColor(petalArray, 10, false);
	colorWheelSnippetFade(petalArray, 10,   1, 2,    125, 235);
}

void SproutLightPatterner::drawStem() {
//	chaseColor(stemArray, 5, 6, 0, 1, 0);
//	dialedColor(stemArray, 5, false);
	colorWheelSnippetFade(stemArray, 5,     4, 4,      0, 40);
}


void SproutLightPatterner::colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize, int patternSpeedModifier,
		int brightnessSpeedModifier, byte wheelBegin, byte wheelEnd, bool wrap /*false*/) {
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
		long timeBetweenBrightnesses = 5 * brightnessSpeedModifier;
		int totalBrightSteps = 628;
		int brightnessAngleIndex = (currTime%(totalBrightSteps*timeBetweenBrightnesses))/timeBetweenBrightnesses;
		float cosBright = cos(brightnessAngleIndex/100.0);
		brightness = (cosBright + 1.0)*1800.0; // never turn it all the way off
	}

	long dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
	long colorTimeBetweenSteps = 2 * patternSpeedModifier * dialSpeed;

	int stepRange = wheelEnd-wheelBegin;
	int totalSteps = stepRange;
	if(wrap == false)
		totalSteps = stepRange*2;

	int offset = totalSteps / arraySize;
	int patternIndex = (currTime%(totalSteps*colorTimeBetweenSteps))/colorTimeBetweenSteps;

	for(int i=0; i<arraySize; i++) {
		int currIndex = patternIndex + offset*i;
		if(currIndex > totalSteps)
			currIndex -= totalSteps;

		byte wheelStepPosition = currIndex<=stepRange ? currIndex : (stepRange - (currIndex-stepRange));

		singleMan->lightMan()->setColorToChannelFromWheelPosition(colorArray[i], wheelBegin+wheelStepPosition, brightness);
	}
}


void SproutLightPatterner::chaseColor(TLC_CHANNEL_TYPE colorArray[], int arraySize, int patternSpeedModifier,
		double redMultiplier, double greenMultiplier, double blueMultiplier) {
	unsigned long currTime = millis();

	long dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
	long colorTimeBetweenSteps = patternSpeedModifier * dialSpeed;

	int midSteps = 250;
	int totalSteps = midSteps*2;
	int offset = totalSteps / arraySize;
	int patternIndex = (currTime%(totalSteps*colorTimeBetweenSteps))/colorTimeBetweenSteps;

	for(int i=0; i<arraySize; i++) {

		int currIndex = patternIndex + offset*i;
		if(currIndex > totalSteps)
			currIndex -= totalSteps;

		byte brightness = currIndex<midSteps ? currIndex : (midSteps - (currIndex-midSteps));

		singleMan->lightMan()->setColorToChannel(colorArray[i], brightness*redMultiplier,
				brightness*greenMultiplier, brightness*blueMultiplier);
	}
}

void SproutLightPatterner::dialedColor(TLC_CHANNEL_TYPE colorArray[], int arraySize, bool doOutput) {
	byte wheelPos = map(singleMan->inputMan()->getRightDial(), 0, 1023, 0, 255);
	static byte currWheelPos = wheelPos;

	for(int i=0; i<arraySize; i++) {
		singleMan->lightMan()->setColorToChannelFromWheelPosition(colorArray[i], wheelPos);
	}

	if(doOutput && (wheelPos < currWheelPos-1 || currWheelPos+1 < wheelPos)) {
  	currWheelPos = wheelPos;
		info_println(currWheelPos);
	}
}
