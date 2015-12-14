#include "EngineBoard.h"
#include "EngineRotateCallback.h"

using namespace osg;
using namespace osgSim;

namespace Eaagles {

	EngineBoard::EngineBoard() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

	void EngineBoard::apply(Transform& node) {
		Engine* pEngine = dynamic_cast<Engine*>(&node);
		if (pEngine) {
			static int i = 0;
			pEngine->setEngineIndex(i);
			pEngine->setAnimationOn(true);
			pEngine->setIncrementHPR(Vec3(0.0, 0.0, 0.0));
			pEngine->setMaxHPR(Vec3(0.0, 0.0, 45000.0));
			pEngine->setMinHPR(Vec3(0.0, 0.0, 0.0));
			pEngine->setUpdateCallback(new EngineRotateCallback(uav.getRefPtr()));
			mEngines[i] = pEngine;
			i++;
		}
		NodeVisitor::apply(node);
	}

	Engine* EngineBoard::createEngineNode(Node* node, const Vec3& vec) {
		ref_ptr<Engine> transformEngine = new Engine;
		transformEngine->setCurrentTranslate(vec);
		transformEngine->setCurrentScale(Vec3(1, 1, 1));
		transformEngine->addChild(node);
		return transformEngine.release();
	}

	void EngineBoard::setupAircraft(Simulation::UnmannedAirVehicle* pav) {
		uav = pav;
	}
}