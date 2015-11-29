//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "Factory.h"
#include "IoHandler.h"
#include "SimStation.h"
#include "Display.h"
#include "Map.h"
#include "Hsi.h"
#include "Pfd.h"
#include "NetFDM.h"

#include <openeaagles/basic/Object.h>

// class factories
#include <openeaagles/basic/Factory.h>
#include <openeaagles/basicGL/Factory.h>
#include <openeaagles/gui/glut/Factory.h>
#include <openeaagles/instruments/Factory.h>
#include <openeaagles/ioDevice/Factory.h>
#include <openeaagles/simulation/Factory.h>
#include <openeaagles/dynamics/Factory.h>

#include <cstring>

namespace Eaagles {

	Factory::Factory(){}

	Basic::Object* Factory::createObj(const char* name) {
		Basic::Object* obj = 0;
		if ( std::strcmp(name, SimStation::getFactoryName()) == 0 ) {
			obj = new SimStation();
		}
		if ( std::strcmp(name, Pfd::getFactoryName()) == 0 ) {
			obj = new Pfd;
		}
		if (std::strcmp(name, Map::getFactoryName()) == 0 ) {
			obj = new Map;
		}
		if (std::strcmp(name, Hsi::getFactoryName()) == 0 ) {
			obj = new Hsi;
		}
		if ( std::strcmp(name, Display::getFactoryName()) == 0 ) {
			obj = new Display();
		}
		if ( std::strcmp(name, IoHandler::getFactoryName()) == 0 ) {
			obj = new IoHandler();
		}
		if (std::strcmp(name, NetFDM::getFactoryName()) == 0) {
			obj = new NetFDM();
		}
		if (obj == 0) 
			obj = Eaagles::Simulation::Factory::createObj(name);
		if (obj == 0) 
			obj = Instruments::Factory::createObj(name);
		if (obj == 0) 
			obj = IoDevice::Factory::createObj(name);
		if (obj == 0) 
			obj = Dynamics::Factory::createObj(name);
		if (obj == 0) 
			obj = BasicGL::Factory::createObj(name);
		if (obj == 0) 
			obj = Glut::Factory::createObj(name);
		if (obj == 0) 
			obj = Basic::Factory::createObj(name);

		return obj;
	}
}  // end namespace Eaagles

