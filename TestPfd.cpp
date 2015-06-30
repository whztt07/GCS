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
		cmdHdg = 50;

		send("cmdhdg", UPDATE_VALUE, cmdHdg, headingSD);
		pitch = av->getPitchD();
		roll = av->getRollD();
		send("adi", UPDATE_INSTRUMENTS, pitch, pitchSD);
    send("adi", UPDATE_VALUE, roll, rollSD);
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
