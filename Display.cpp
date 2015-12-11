#include "Display.h"
#include <openeaagles/basic/PairStream.h>

namespace Eaagles {

	IMPLEMENT_SUBCLASS(Display,"Display")
	EMPTY_SLOTTABLE(Display)
	EMPTY_SERIALIZER(Display)
	EMPTY_DELETEDATA(Display)
	
	Display::Display() {
		STANDARD_CONSTRUCTOR()
	}

	// copy member data
	void Display::copyData(const Display& org, const bool) {
		BaseClass::copyData(org);
	}

	//------------------------------------------------------------------------------
	// shutdownNotification() -- Shutdown the simulation
	//------------------------------------------------------------------------------
	bool Display::shutdownNotification() {
		 Basic::Component* parent = container();
		 if (parent != 0) 
			 parent->event(SHUTDOWN_EVENT);
		 return BaseClass::shutdownNotification();
	}

	//------------------------------------------------------------------------------
	// updateData() -- update non-time critical stuff here
	//------------------------------------------------------------------------------

	void Display::updateData(const LCreal dt) {
		 // Update base classes stuff
		 BaseClass::updateData(dt);
	}

	void Display::reshapeIt(int w, int h) {
		// Update base classes stuff
		BaseClass::reshapeIt(w,h);
	}
} // End Eaagles namespace

