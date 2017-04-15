#include "SproutLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

SproutLightPatterner::SproutLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

}

void SproutLightPatterner::drawPattern() {
	unsigned long currTime =  millis();

	int dialSpeed = map(singleMan->inputMan()->getRightDial(), 0, 1023, 10, 1);
	byte colorTimeBetweenSteps = 3 * dialSpeed;

	byte wheelPos = (currTime%(COLOR_STEPS_IN_WHEEL*colorTimeBetweenSteps))/colorTimeBetweenSteps;
	byte offsetForLeds = COLOR_STEPS_IN_WHEEL / singleMan->lightMan()->getTotalChannels();

	for(int i=0; i<singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannelFromWheelPosition(singleMan->lightMan()->channelArray[i], wheelPos+(offsetForLeds*i));
	}
}
