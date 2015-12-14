#include "EngineRotateCallback.h"
#include "Engine.h"

using namespace osg;

namespace Eaagles {

	EngineRotateCallback::EngineRotateCallback(Simulation::UnmannedAirVehicle* pav) : uav(pav) {}
	
	void EngineRotateCallback::operator()(Node* node, NodeVisitor* nv) {
		Engine* pEngine = dynamic_cast<Engine*>(node);
		if (pEngine != nullptr && uav != nullptr) {
			LCreal rpm[NUMBER_OF_ENGINES];
			int engmax = uav->getEngRPM(rpm, NUMBER_OF_ENGINES);
			if (engmax != NUMBER_OF_ENGINES)
				return;
			pEngine->setIncrementHPR(Vec3(0.0, 0.0, pEngine->getEngineIndex()));
		}
		traverse(node, nv);
	}
}