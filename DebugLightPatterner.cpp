#include "DebugLightPatterner.h"

#include <Arduino.h>

#include "Tlc5940.h"
#include "ProjectDefs.h"
#include "SingletonManager.h"

DebugLightPatterner::DebugLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

}

void DebugLightPatterner::drawPattern() {
	int blinkSpeed=250;

	for(int i=0; i < singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 0, 0, 0);
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

}

void DebugLightPatterner::testPatternRGBAll(int blinkSpeed) {
	int totalColors = 3;
	byte colorIndex = (millis()%(totalColors*blinkSpeed))/blinkSpeed;

	byte red=0, green=0, blue=0;
	if(colorIndex == 0) red = 255;
	else if (colorIndex == 1) green = 255;
	else blue = 255;

	for(int i=0; i<singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], red, green, blue);
	}
}

void DebugLightPatterner::testPatternStationCount(int blinkSpeed) {
	byte stationIndex = (millis()%(NUM_TLCS*blinkSpeed))/blinkSpeed;

	byte channelOffset = stationIndex*5;
	for(int i=channelOffset; i<channelOffset+5; i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 255, 255);
	}
}

void DebugLightPatterner::testPatternStationChannelCount(int blinkSpeed) {
	byte stationIndex = (millis()%(5*blinkSpeed))/blinkSpeed;

	for(int i=stationIndex; i<singleMan->lightMan()->getTotalChannels(); i+=5) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 255, 255);
	}
}

void DebugLightPatterner::testPatternFullCount(int blinkSpeed) {
	byte fullCounts = NUM_TLCS*16;
	byte channelIndex = (millis() % ((long)fullCounts*(long)blinkSpeed)) / (long) blinkSpeed;
	Tlc.set(channelIndex, 0);
}
