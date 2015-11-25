#include "Hsi.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Hsi,"Hsi")
	EMPTY_SERIALIZER(Hsi)

	Hsi::Hsi() {
		STANDARD_CONSTRUCTOR()
		heading = 0;
		headingSD.empty();
	}

	void Hsi::copyData(const Hsi& org, const bool)	{
		BaseClass::copyData(org);
		heading = org.heading;
    headingSD.empty();
	}

	EMPTY_DELETEDATA(Hsi)

	void Hsi::updateData(const LCreal dt) {
		Simulation::AirVehicle* av = static_cast<Simulation::AirVehicle*>(getOwnship());
		heading = av->getHeadingD();
    send("hsi1", UPDATE_VALUE6, heading, headingSD);
		BaseClass::updateData(dt);
	}

	Simulation::Player* Hsi::getOwnship()	{
		Simulation::Player* p = 0;
		Simulation::Station* sta = getStation();
		if (sta != 0) p = sta->getOwnship();
		return p;
	}

	Simulation::Simulation* Hsi::getSimulation() {
	 Simulation::Simulation* s = 0;
	 Simulation::Station* sta = getStation();
	 if (sta != 0) s = sta->getSimulation();
	 return s;
	}

	Simulation::Station* Hsi::getStation() {
	 if (myStation == 0) {
			Simulation::Station* s = dynamic_cast<Simulation::Station*>( findContainerByType(typeid(Simulation::Station)) );
			if (s != 0) myStation = s;
	 }
	 return myStation;
	}
} // end of Eaagles namespace
