#ifndef TEST_PFD
#define TEST_PFD

#include <openeaagles/basicGL/Page.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/simulation/AirVehicle.h>
#include "SimStation.h"

namespace Eaagles {
	class TestPfd : public BasicGL::Page 	{
	DECLARE_SUBCLASS(TestPfd,BasicGL::Page)
	public:
		TestPfd();
	  // Basic::Component interface
		virtual void updateData(const LCreal dt = 0);
	private:

		Simulation::Player* getOwnship();
		Simulation::Simulation* getSimulation();
		Simulation::Station* getStation();

		SPtr<Simulation::Station> myStation;

		LCreal          range;          // SD range
		SendData        headingSD;
    SendData        rangeSD;

		// pitch and roll
    LCreal pitch;           // degs
		SendData        pitchSD;
    LCreal pitchRate;       // degs/sec
    LCreal roll;            // degs
		SendData        rollSD;
    LCreal rollRate;        // degs/sec

    // heading and nav stuff
    LCreal trueHdg;         // degs
    LCreal tHdgRate;        // degs/sec
    LCreal cmdHdg;          // commanded heading (heading bug) (degs)
    LCreal cmdHdgRate;      // degs/sec

    // airspeed
    LCreal airSpd;
    LCreal airSpdRate;

    // altitude
    LCreal alt;
    LCreal altRate;

    // side slip
    LCreal slip;            // degs
    LCreal slipRate;        // degs/sec

    // glideslope (in dots)
    LCreal gSlope;
    LCreal gSlopeRate;

    // Lateral dev
    LCreal latDev;
    LCreal ldRate;

    // commanded speed
    LCreal cmdSpd;

    // commanded alt
    LCreal cmdAlt;

    // vvi
    LCreal vvi;
    LCreal vviRate;

    // flight director stuff (in inches)
    LCreal fDirBank;
    LCreal fDirBankRate;
    LCreal fDirPitch;
    LCreal fDirPitchRate;
    
    // barometric pressure
    LCreal baro;
    LCreal baroRate;
	};
} // end of Eaagles namespace

#endif

