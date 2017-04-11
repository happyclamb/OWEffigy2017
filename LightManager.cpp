#include "LightManager.h"

#include <Arduino.h>

#include "Tlc5940.h"
#include "ProjectDefs.h"
#include "SingletonManager.h"

LightManager::LightManager(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	Tlc.init(4095);

	for(int i=0;i<NUM_TLCS;i++) {
		for(int j=0;j<5;j++) {
			channelArray[(i*5)+j] = (i*16)+(j*3);

			info_print((i*5)+j);
			info_print(": ");
			info_println(channelArray[(i*5)+j]);
		}
  }

	singleMan->setLightMan(this);
}


void LightManager::testPattern() {
	int blinkSpeed=400;

	for(int i=0;i<totalChannels;i++) {
		this->setColorToChannel(channelArray[i], 0, 0, 0);
	}

	switch(map(singleMan->inputMan()->getRightDial(), 0, 1023, 0, 3))
	{
		case 0:
			testPatternRGBAll(blinkSpeed);
			break;
		case 1:
			testPatternStationCount(blinkSpeed);
			break;
		case 2:
			testPatternStationChannelCount(blinkSpeed);
			break;
		case 3:
			testPatternFullCount(blinkSpeed);
			break;
		}

	Tlc.update();
}

void LightManager::testPatternRGBAll(int blinkSpeed) {
	int totalColors = 3;
	byte colorIndex = (millis()%(totalColors*blinkSpeed))/blinkSpeed;

	byte red=0, green=0, blue=0;
	if(colorIndex == 0) red = 255;
	else if (colorIndex == 1) green = 255;
	else blue = 255;

	for(int i=0;i<totalChannels;i++) {
		this->setColorToChannel(channelArray[i], red, green, blue);
	}
}

void LightManager::testPatternStationCount(int blinkSpeed) {
	byte stationIndex = (millis()%(NUM_TLCS*blinkSpeed))/blinkSpeed;

	byte channelOffset = stationIndex*5;
	for(int i=channelOffset; i<channelOffset+5; i++) {
		this->setColorToChannel(channelArray[i], 255, 255, 255);
//		info_print(i);
//		info_print("  ");
	}
//	info_println(" ");
}

void LightManager::testPatternStationChannelCount(int blinkSpeed) {
	byte stationIndex = (millis()%(5*blinkSpeed))/blinkSpeed;

	for(int i=stationIndex;i<totalChannels;i+=5) {
		this->setColorToChannel(channelArray[i], 255, 255, 255);
//		info_print(i);
//		info_print("  ");
	}
//	info_println(" ");
}

void LightManager::testPatternFullCount(int blinkSpeed) {

//	byte channelIndex = (millis()%(totalChannels*blinkSpeed))/blinkSpeed;
	byte fullCounts = NUM_TLCS*16;
	byte channelIndex = (millis() % ((long)fullCounts*(long)blinkSpeed)) / (long) blinkSpeed;
	Tlc.set(channelIndex, 0);
}


void LightManager::setColorToChannel(TLC_CHANNEL_TYPE channelToSet, byte red, byte green, byte  blue) {
	int brightness = map(singleMan->inputMan()->getLeftDial(), 0, 1023, 4095, 0);

	Tlc.set(channelToSet+0, map(red  , 0, 255, 4095, brightness));
	Tlc.set(channelToSet+1, map(green, 0, 255, 4095, brightness));
	Tlc.set(channelToSet+2, map(blue , 0, 255, 4095, brightness));
}

void LightManager::setColorToChannelFromWheelPosition(TLC_CHANNEL_TYPE channelToSet, byte wheelPos) {
	byte r=0;
	byte g=0;
	byte b=0;
	this->colorFromWheelPosition(wheelPos, &r, &g, &b);
	this->setColorToChannel(channelToSet, r, g, b);
}

void LightManager::colorFromWheelPosition(byte wheelPos, byte *r, byte *g, byte *b) {
	if (wheelPos < 85) {
		// 0-->85
		*r = wheelPos * 3;
		*g = 255 - wheelPos * 3;
		*b = 0;
	} else if (wheelPos < 170) {
		// 85-->170
		wheelPos -= 85;
		*r = 255 - wheelPos * 3;
		*g = 0;
		*b = wheelPos * 3;
	} else {
		// 170-->255
		wheelPos -= 170;
		*r = 0;
		*g = wheelPos * 3;
		*b = 255 - wheelPos * 3;
	}
}


// Treat all 6 LEDs as 1 solid colour, default pattern look is to have all
//  the lanterns showing exactly the same pattern PATTERN_TIMING_SYNC
//	if (allLaternLEDs) then make all 6 LEDs the same
// MATH: cycle through all 255 wheel positions over ~3sec
// == 3000/255 == 11.7 ... make a step 12ms == 12*255 == 3060
void LightManager::solidWheelColorChange() {
	unsigned long currTime =  millis();


	int dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
	byte colorTimeBetweenSteps = 3 * dialSpeed;  // 5 -> 50

	byte wheelPos = (currTime%(COLOR_STEPS_IN_WHEEL*colorTimeBetweenSteps))/colorTimeBetweenSteps;
	byte offsetForLeds = COLOR_STEPS_IN_WHEEL / totalChannels;

	for(int i=0;i<totalChannels;i++) {
		this->setColorToChannelFromWheelPosition(channelArray[i], wheelPos+(offsetForLeds*i));
	}

	Tlc.update();
}

/*
void LightManager::sinWheelColorChange() {
	unsigned long currTime =  millis();



	int dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
	byte colorTimeBetweenSteps = 3 * dialSpeed;  // 5 -> 50


	byte wheelPos = (currTime%(COLOR_STEPS_IN_WHEEL*colorTimeBetweenSteps))/colorTimeBetweenSteps;


	this->setColorToChannelFromWheelPosition(CH_01, wheelPos);
	Tlc.update();
}
*/
