#ifndef __SPROUTLIGHTPATTERNER_H__
#define __SPROUTLIGHTPATTERNER_H__

#include <Arduino.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class SproutLightPatterner {
	public:
		SproutLightPatterner(SingletonManager* _singleMan);

		void drawPattern();

	private:
		SingletonManager* singleMan = NULL;

		// Need to hold channel assignments for Seed (5); Petal (10); Stem(5); Hub(5)
		TLC_CHANNEL_TYPE seedArray[5];
		TLC_CHANNEL_TYPE petalArray[10];
		TLC_CHANNEL_TYPE stemArray[5];
		TLC_CHANNEL_TYPE hubArray[5];

		unsigned long choosePattern(int currPattern, byte patternVar);
		unsigned long pattern_00(byte solidColour);
		unsigned long pattern_01(byte unused);
		unsigned long pattern_02(byte unused);

		void colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize, int patternSpeedModifier,
				int brightnessSpeedModifier, byte wheelBegin, byte wheelEnd, bool wrap=false);
};

#endif // __SPROUTLIGHTPATTERNER_H__
