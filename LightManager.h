#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__

#include <Arduino.h>
#include <Tlc5940.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class LightManager {
	public:

		LightManager(SingletonManager* _singleMan);

		void testPattern(); // called from interrupt handler

		void setColorToChannel(TLC_CHANNEL_TYPE channelToSet, byte red, byte green, byte  blue);
		void colorFromWheelPosition(byte wheelPos, byte *r, byte *g, byte *b);
		void setColorToChannelFromWheelPosition(TLC_CHANNEL_TYPE channelToSet, byte wheelPos);

		void solidWheelColorChange();

	private:
		SingletonManager* singleMan = NULL;
		int totalChannels = NUM_TLCS*5;
		TLC_CHANNEL_TYPE channelArray[NUM_TLCS*5];

		void testPatternRGBAll(int blinkSpeed);
		void testPatternStationCount(int blinkSpeed);
		void testPatternStationChannelCount(int blinkSpeed);
		void testPatternFullCount(int blinkSpeed);
};

#endif // __LIGHTMANAGER_H__
