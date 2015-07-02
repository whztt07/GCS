#include "TestPfd.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestPfd,"TestPfd")
	EMPTY_SERIALIZER(TestPfd)

	TestPfd::TestPfd() {
		STANDARD_CONSTRUCTOR()
	}
	
	void TestPfd::copyData(const TestPfd& org, const bool) {
		BaseClass::copyData(org);
	}

	EMPTY_DELETEDATA(TestPfd)

	void TestPfd::updateData(const LCreal dt) {
		Simulation::AirVehicle* av = static_cast<Simulation::AirVehicle*>(getOwnship());
		
		trueHdg = av->getHeadingD();
		if (trueHdg > 360) 
			trueHdg = 0;
		// selected heading
		cmdHdg = 0;
		if (cmdHdg > 360)
			cmdHdg = 0;
		
		pitch = av->getPitchD();
		
		roll = av->getRollD();
		
		alt = av->getAltitudeFt();

		airSpd = av->getCalibratedAirspeed();
		// test data
		LCreal mach = av->getMach();
		// commanded speed
		cmdSpd = 150;

		// here is sideslip
		slip = av->getSideSlipD();

		LCreal altDiff = alt - cmdAlt;
    // let's break the altitude down into ones and tens, so we can
    // send that data to the tape gauge
    LCreal altTens = ((alt/100) - static_cast<int>(alt/100)) * 10;
    // now figure the rest of the number
    int altRest = static_cast<int>(alt/99.9999);

		 // find the last digit for the readout tape
    LCreal ones = ((airSpd / 10) - static_cast<int>(airSpd / 10)) * 10;
    // find the 100s value for the dynamic arc segment
    int rest = static_cast<int>(airSpd / 10.0f);
    
    LCreal diff = airSpd - cmdSpd;

		// commanded alt
		cmdAlt = 1500;
		// glideslope
		gSlope = 0;
		// lat dev
		latDev = 0;
		// vvi tape gauge test
		const osg::Vec3 vel = av->getVelocity();
		LCreal vvMps = -vel[2];
		vvi = vvMps * 60.0f * Basic::Distance::M2FT;
		// flight director stuff 
		// flight diretor bank angle
		fDirBank = 0;
		// flight director pitch angle
		fDirPitch = 0;
		// barometric pressure (selected)
		baro = 29.92;

		send("adi", UPDATE_INSTRUMENTS, pitch, pitchSD);
    send("adi", UPDATE_VALUE, roll, rollSD);
		//send("hsi", UPDATE_VALUE, trueHdg, headingSD);

		if (altRest < 10) {
        send("altsel", SELECT, 1, altSelectSD);
        send("altrest1", UPDATE_VALUE, altRest, alt1SD);
    }
    else if (altRest < 100) {
        send("altsel", SELECT, 2, altSelectSD);
        send("altrest2", UPDATE_VALUE, altRest, alt2SD);
    }
    else {
        send("altsel", SELECT, 3, altSelectSD);
        send("altrest3", UPDATE_VALUE, altRest, alt3SD);
    }

		send("cmdspd", UPDATE_VALUE, cmdSpd, aBugSD);
    // actual airspeed tape, not just the readout
    send("airspd", UPDATE_INSTRUMENTS, airSpd, airSpdTpSD);
		
		send("cmdhdg", UPDATE_VALUE, cmdHdg, cmdHdgROSD);
    send("hsi", UPDATE_VALUE6, trueHdg, trueHdgSD);
    send("selhdg", UPDATE_VALUE2, cmdHdg, cmdHdgSD);
    send("spdtape", UPDATE_INSTRUMENTS, ones, onesSD);
    send("spdrest", UPDATE_VALUE, rest, spdRstSD);
    send("fdirbnkang", UPDATE_INSTRUMENTS, fDirBank, fDirBankSD);
    send("baro", UPDATE_VALUE, baro, baroSD);
    send("fdirpitch", UPDATE_INSTRUMENTS, fDirPitch, fDirPitchSD);
    send("vvi", UPDATE_INSTRUMENTS, vvi, vviSD);
    send("vviro", UPDATE_VALUE, vvi, vviROSD);
    send("latdev", UPDATE_INSTRUMENTS, latDev, latDevSD);

		send("glideslope", UPDATE_INSTRUMENTS, gSlope, gSlopeSD);
    send("alttens", UPDATE_INSTRUMENTS, altTens, altTensSD);
		send("alttape", UPDATE_INSTRUMENTS, alt, altTpSD);
    send("altbug", UPDATE_INSTRUMENTS, altDiff, altDiffSD);
    send("cmdalt", UPDATE_VALUE, cmdAlt, altBugSD);
    send("spdbug", UPDATE_INSTRUMENTS, diff, diffSD);
		
		send("bankscale", UPDATE_INSTRUMENTS, roll, bascaleSD);
    // send our bank angle the roll
    send("bankangle", UPDATE_INSTRUMENTS, -roll, baSD);
    // sideslip 
    send("sideslip", UPDATE_INSTRUMENTS, slip, slipSD);   
    // send our ghost horizon data
    send("ghosthorizonbar", UPDATE_INSTRUMENTS, pitch, pitchGhostSD);
    // convert alt to meters and send it to our meters readout
    int mAlt = static_cast<int>(Basic::Distance::FeetToMeters(alt));
    LCreal mAltBug = Basic::Distance::FeetToMeters(cmdAlt);
    send("malt", UPDATE_VALUE, mAlt, mAltSD);
    send("cmdmalt", UPDATE_VALUE, mAltBug, cmdMAltSD);

		BaseClass::updateData(dt);
	}

	Simulation::Player* TestPfd::getOwnship()	{
		 Simulation::Player* p = 0;
		 Simulation::Station* sta = getStation();
		 if (sta != 0) p = sta->getOwnship();
		 return p;
	}

	Simulation::Simulation* TestPfd::getSimulation() {
		 Simulation::Simulation* s = 0;
		 Simulation::Station* sta = getStation();
		 if (sta != 0) s = sta->getSimulation();
		 return s;
	}

	Simulation::Station* TestPfd::getStation() {
		 if (myStation == 0) {
				Simulation::Station* s = dynamic_cast<Simulation::Station*>( findContainerByType(typeid(Simulation::Station)) );
				if (s != 0) myStation = s;
		 }
		 return myStation;
	}
} // end of Eaagles namespace
