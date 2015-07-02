#ifndef TEST_HSI
#define TEST_HSI

#include <openeaagles/basicGL/Page.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/simulation/AirVehicle.h>
#include "SimStation.h"

namespace Eaagles {
	class TestHsi : public BasicGL::Page {
	DECLARE_SUBCLASS(TestHsi,BasicGL::Page)
	public:
    TestHsi();
		// Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
	private:
		Simulation::Player* getOwnship();
		Simulation::Simulation* getSimulation();
		Simulation::Station* getStation();

		SPtr<Simulation::Station> myStation;

		LCreal heading;             // our heading 
    SendData headingSD;
    LCreal  headingRate;        // rate which are going (up or down)
    SendData headingROSD;
    SendData dmeROSD;           // dme
    LCreal dme;
	};
} // end of Eaagles namespace

#endif

