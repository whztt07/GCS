#include "Engine.h"

using namespace osg;
using namespace osgSim;

namespace Eaagles {

	Engine::Engine() : DOFTransform() {}

	unsigned int Engine::getEngineIndex() const {
		return mEngineIndex;
	}

	void Engine::setEngineIndex(unsigned int EngineIndex) {
		mEngineIndex = EngineIndex;
	}
}