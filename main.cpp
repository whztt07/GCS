//------------------------------------------------------------------------------
// Demo of sub-displays
//------------------------------------------------------------------------------
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Timers.h"
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basicGL/Graphic.h"

#include "openeaagles/gui/glut/GlutDisplay.h"
#include <GL/glut.h>

// class factories
#include "xPanel/Factory.h"

// class factories
#include "../shared/xZeroMQHandlers/Factory.h"
#include "openeaagles/basic/Factory.h"
#include "openeaagles/basicGL/Factory.h"
#include "openeaagles/gui/glut/Factory.h"
#include "openeaagles/dis/Factory.h"
#include "openeaagles/instruments/Factory.h"
#include "openeaagles/ioDevice/Factory.h"
#include "openeaagles/otw/Factory.h"
#include "openeaagles/sensors/Factory.h"
#include "openeaagles/simulation/Factory.h"
#include "openeaagles/vehicles/Factory.h"

#include "Factory.h"
#include "SimStation.h"

namespace Eaagles {
	
	// Description (input) File -- After being processed by the C preprocessor
	const char* testFileName = "../test.edl";
	// Frame Rate
	const int frameRate = 1;
	// Top level Station
	static Eaagles::Simulation::Station* station = 0;

	// build a station
	static Eaagles::Simulation::Station* builder(const char* const fileName) 	{
		Eaagles::Simulation::Station* p = 0;
		// Read the description file
		int errors = 0;
		Eaagles::Basic::Object* q1 = Eaagles::Basic::lcParser(fileName, Eaagles::Factory::createObj, &errors);
		if (errors > 0) {
			std::cerr << "File: " << fileName << ", errors: " << errors << std::endl;
			return 0;
		}
		if (q1 != 0) {
			// When we were given a Basic::Pair, get the pointer to its object.
			Eaagles::Basic::Pair* pp = dynamic_cast<Eaagles::Basic::Pair*>(q1);
			if (pp != 0) {
				q1 = pp->object();
			}
			// What we should have here is the Station object
			p = dynamic_cast<Eaagles::Simulation::Station*>(q1);
		}
		return p;
	}
		
	static void updateDataCB(int msecs) {
		// Current time
		double time = Eaagles::getComputerTime();
		// Compute delta time
		static double time0 = time;   // N-1 Time
		Eaagles::LCreal dt = static_cast<Eaagles::LCreal>(time - time0);
		time0 = time;
		station->updateData(dt);
	}
	
	int main(int argc, char* argv[]) {
		glutInit(&argc, argv);
	  
		station = builder(testFileName);
		if (station == 0) {
			std::cerr << "Invalid configuration file!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		station->event(Eaagles::Basic::Component::RESET_EVENT);
		
		double dt = 1.0/static_cast<double>(frameRate);
		int msecs = static_cast<int>(dt * 1000);

		station->updateData(dt);
		station->updateTC(dt);
		station->event(Eaagles::Basic::Component::RESET_EVENT);

		glutTimerFunc(msecs, updateDataCB, 1);

		station->createTimeCriticalProcess();

		glutMainLoop();
		return 0;
	}
}

int main(int argc, char* argv[]) {
	return Eaagles::main(argc, argv);
}