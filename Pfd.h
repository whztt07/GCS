#ifndef PFD
#define PFD

#include <openeaagles/gui/glut/GlutDisplay.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/simulation/AirVehicle.h>
#include "SimStation.h"

namespace Eaagles {
	class Pfd : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(Pfd, Glut::GlutDisplay)
	public:
		Pfd();
	  // Basic::Component interface
		virtual void updateData(const LCreal dt = 0);

	private:
		Simulation::Player* getOwnship();
		Simulation::Simulation* getSimulation();
		Simulation::Station* getStation();

		Basic::safe_ptr<Simulation::Station> myStation;

		LCreal        range;          // SD range
		SendData      headingSD;
    SendData      rangeSD;

    LCreal				pitch;	   // degs
		LCreal				pitchRate; // degs/sec
		SendData      pitchSD;
    
    LCreal				roll;      // degs
		LCreal				rollRate;  // degs/sec
		SendData			rollSD;
    
		 // heading and nav stuff
    LCreal				trueHdg;         // degs
		LCreal				tHdgRate;        // degs/sec
    LCreal				cmdHdg;          // commanded heading (heading bug) (degs)
    LCreal				cmdHdgRate;      // degs/sec
		SendData			tHdgSD;
		SendData			cmdHdgROSD;
		SendData			trueHdgSD;
    SendData			hdgTapeSD;
    SendData			cmdHdgSD;

    // airspeed
    LCreal				airSpd;
    LCreal				airSpdRate;
		SendData			airSpdTpSD;    // for the airspeed tape
    SendData			onesSD;        // for the readout ones tape
    SendData			spdRstSD;      // rest of the speed data

		// altitude
    LCreal				alt;
    LCreal				altRate;
    SendData			alt1SD;
    SendData			alt2SD;
    SendData			alt3SD;
    SendData			altTpSD;       // for the airspeed tape
    SendData			altTensSD;
    SendData			altSelectSD;   // to select the justification
    SendData			altSD;         // for the actual altitude
		SendData			altBugSD;
    SendData			altDiffSD;

    // side slip
    LCreal				slip;            // degs
    LCreal				slipRate;        // degs/sec
		SendData			slipSD;

    // glideslope (in dots)
    LCreal				gSlope;
    LCreal				gSlopeRate;
		SendData			gSlopeSD;

    // Lateral dev
    LCreal				latDev;
    LCreal				ldRate;
		SendData			latDevSD;

    // commanded speed kts
    LCreal				cmdSpd;
    SendData			aBugSD;
    SendData			diffSD;
    // commanded alt
    LCreal				cmdAlt;

    // vvi
    LCreal				vvi;
    LCreal				vviRate;
		SendData			vviSD;
    SendData			vviROSD;

    // flight director stuff (in inches)
    LCreal				fDirBank;
    LCreal				fDirBankRate;
		SendData			fDirBankSD;
    LCreal				fDirPitch;
    LCreal				fDirPitchRate;
    SendData			fDirPitchSD;
		// bank angle
    SendData			baSD;
    SendData			bascaleSD;

    // barometric pressure
    LCreal				baro;
    LCreal				baroRate;
		SendData			baroSD;

		// Gs
    LCreal				gLoad;
    // Mach
    LCreal				mach;    //// meter altitude
    SendData			mAltSD;
    SendData			cmdMAltSD;

    // ghost horizon
    SendData pitchGhostSD;
	};
} // end of Eaagles namespace

#endif