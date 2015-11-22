#ifndef TEST_HSI
#define TEST_HSI

#include <openeaagles/gui/glut/GlutDisplay.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/simulation/AirVehicle.h>
#include "SimStation.h"

namespace Eaagles {
	class TestHsi : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(TestHsi, Glut::GlutDisplay)
	public:
    TestHsi();
		// Basic::Component interface
    virtual void updateData(const LCreal dt = 0);

	private:
		Simulation::Player* getOwnship();
		Simulation::Simulation* getSimulation();
		Simulation::Station* getStation();

		Basic::safe_ptr<Simulation::Station> myStation;

		LCreal heading;             // our heading 
    SendData headingSD;
	};
} // end of Eaagles namespace

#endif

