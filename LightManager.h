#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__

#include <Arduino.h>
#include <Tlc5940.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

#include "DebugLightPatterner.h"
class DebugLightPatterner;
#include "SproutLightPatterner.h"
class SproutLightPatterner;

class LightManager {
	public:

		LightManager(SingletonManager* _singleMan);

		void updateLights(); // called from interrupt handler

		void setColorToChannel(TLC_CHANNEL_TYPE channelToSet, byte red, byte green, byte  blue);
		void colorFromWheelPosition(byte wheelPos, byte *r, byte *g, byte *b);
		void setColorToChannelFromWheelPosition(TLC_CHANNEL_TYPE channelToSet, byte wheelPos);

		void solidWheelColorChange();

		int getTotalChannels() { return this->totalChannels; }
		// GROSS: Forgot Arrays are 2nd class citizens in C - cheated and made it public
		TLC_CHANNEL_TYPE channelArray[NUM_TLCS*5];

	private:
		SingletonManager* singleMan = NULL;
		int totalChannels = NUM_TLCS*5;

		DebugLightPatterner* debugPatterner = NULL;
		SproutLightPatterner* sproutPatterner = NULL;

};

#endif // __LIGHTMANAGER_H__
