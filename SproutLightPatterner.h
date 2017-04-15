#ifndef __SPROUTLIGHTPATTERNER_H__
#define __SPROUTLIGHTPATTERNER_H__

#include <Arduino.h>
#include <Tlc5940.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class SproutLightPatterner {
	public:
		SproutLightPatterner(SingletonManager* _singleMan);

		void drawPattern();

	private:
		SingletonManager* singleMan = NULL;

};

#endif // __SPROUTLIGHTPATTERNER_H__
