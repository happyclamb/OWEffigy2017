#ifndef __DEBUGLIGHTPATTERNER_H__
#define __DEBUGLIGHTPATTERNER_H__

#include <Arduino.h>
#include <Tlc5940.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class DebugLightPatterner {
	public:
		DebugLightPatterner(SingletonManager* _singleMan);

		void drawPattern();

	private:
		SingletonManager* singleMan = NULL;

		void testPatternRGBAll(int blinkSpeed);
		void testPatternStationCount(int blinkSpeed);
		void testPatternStationChannelCount(int blinkSpeed);
		void testPatternFullCount(int blinkSpeed);
};

#endif // __DEBUGLIGHTPATTERNER_H__
