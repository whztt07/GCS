#ifndef HSI
#define HSI

#include <openeaagles/gui/glut/GlutDisplay.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/simulation/AirVehicle.h>
#include "SimStation.h"

namespace Eaagles {
	class Hsi : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(Hsi, Glut::GlutDisplay)
	public:
    Hsi();
		// Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
		//Basic::Component interface
		virtual bool event(const int event, Basic::Object* const obj = nullptr) override;
		//Hsi interface
		bool onEntry();

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

