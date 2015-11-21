#include "TestDisplay.h"
#include "SimStation.h"
#include "xPanel/DspRadar.h"
#include "xPanel/DspRwr.h"
#include "xPanel/Pfd.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Jammer.h"
#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Rwr.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/sensors/Gmti.h"
#include "openeaagles/sensors/Tws.h"
#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basicGL/SymbolLoader.h"
#include <GL/glut.h>

namespace Eaagles {

	IMPLEMENT_SUBCLASS(TestDisplay,"TestDisplay")
	EMPTY_SLOTTABLE(TestDisplay)
	EMPTY_SERIALIZER(TestDisplay)
	EMPTY_DELETEDATA(TestDisplay)
	
	TestDisplay::TestDisplay() {
		STANDARD_CONSTRUCTOR()
	}

	// copy member data
	void TestDisplay::copyData(const TestDisplay& org, const bool) {
		BaseClass::copyData(org);
	}

	//------------------------------------------------------------------------------
	// shutdownNotification() -- Shutdown the simulation
	//------------------------------------------------------------------------------
	bool TestDisplay::shutdownNotification() {
		 Basic::Component* parent = container();
		 if (parent != 0) parent->event(SHUTDOWN_EVENT);
		 return BaseClass::shutdownNotification();
	}

	//------------------------------------------------------------------------------
	// updateData() -- update non-time critical stuff here
	//------------------------------------------------------------------------------

	void TestDisplay::updateData(const LCreal dt) {
		 // Update base classes stuff
		 BaseClass::updateData(dt);
	}

	void TestDisplay::reshapeIt(int w, int h) {
		// Update base classes stuff
		BaseClass::reshapeIt(w,h);
	}
} // End Eaagles namespace

