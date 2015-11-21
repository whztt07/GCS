//------------------------------------------------------------------------------
// Class: TestDisplay
//------------------------------------------------------------------------------
#ifndef TEST_DISPLAY
#define TEST_DISPLAY

#include <openeaagles/gui/glut/GlutDisplay.h>

namespace Eaagles {
	namespace Simulation { class Missile; class Player; class Simulation; class Station; }
  namespace BasicGL { class SymbolLoader; }
  namespace xPanel { class DspRadar; class DspRwr; }

	//------------------------------------------------------------------------------
	// Class: TestDisplay
	// Description: Test GLUT-display that will manage a simple real-beam, b-scan radar
	//              display, plus a Radar receiver display, which shows received
	//              signal strength and angle of arrival, a simple situation
	//              display that shows the 'truth' location of the players, and
	//              Primary Flight Display (PFD).
	//
	// Factory name: TestDisplay
	//
	// Events: (all keyboard events)
	//   'r' or 'R'   -- Reset simulation
	//   'f' or 'F'   -- Toggle simulation freeze
	//   'l' or 'L'   -- Launch Missile
	//   'a' or 'A'   -- A/A mode (TWS)
	//   'g' or 'G'   -- A/G mode (GMTI)
	//   's' or 'S'   -- Target step
	//   'i' or 'I'   -- Increase Range
	//   'd' or 'D'   -- Decrease Range
	//   '+'          -- Ownship step (to next local air vehicle)
	//------------------------------------------------------------------------------
	class TestDisplay : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(TestDisplay, Glut::GlutDisplay)
	public:
		enum { MAX_TRACKS = 60 };
		enum { NCHAR_NAV1_ID = 3, NCHAR_NAV2_ID = 5 };
	public:
		TestDisplay();
		//Basic::Display interface
		virtual void reshapeIt(int w, int h);
		//Basic::Component interface
		virtual void updateData(const LCreal dt = 0.0f);
		// Basic::Component interface
	protected:
		// Basic::Component protected interface
		virtual bool shutdownNotification();
	};
} // End Eaagles namespace

#endif

