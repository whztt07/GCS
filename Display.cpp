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

	BEGIN_EVENT_HANDLER(Display)
		ON_EVENT(Eaagles::Basic::Component::USER_EVENTS + 1, onEntry)
	END_EVENT_HANDLER()

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

	bool Display::onEntry() {
		Basic::PairStream* subcomponents = subDisplays();
		if (subcomponents != nullptr) {
			Basic::List::Item* item = subcomponents->getFirstItem();
			while (item != nullptr) {
				Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
				Basic::Component* cp = static_cast<Basic::Component*>(pair->object());
				if (cp != nullptr) 
					cp->event(Eaagles::Basic::Component::USER_EVENTS + 1);
				item = item->getNext();
			}
		}
		return true;
	}
} // End Eaagles namespace

