#include "EngineVisitor.h"
#include <algorithm>

using namespace osg;
using namespace osgSim;
using namespace osgGA;

namespace Eaagles {

	EngineVisitor::EngineVisitor() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN), uav(nullptr) {}

	EngineVisitor::~EngineVisitor() {}

	void EngineVisitor::apply(Transform& node) {
		DOFTransform* pDof = dynamic_cast<DOFTransform*>(&node);
		static int i = 0;
		if (pDof) {
			pDof->setAnimationOn(true);
			pDof->setIncrementHPR(Vec3(0.0, 0.0, 0.0));
			pDof->setMaxHPR(Vec3(0.0, 0.0, 45000.0));
			pDof->setMinHPR(Vec3(0.0, 0.0, 0.0));
			mEngines[i] = pDof;
		}
		NodeVisitor::apply(node);
	}

	bool EngineVisitor::handle(const GUIEventAdapter& ea, GUIActionAdapter& aa) {
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)
			return false;

		if (ea.getHandled())
			return false;

		if (ea.getEventType() == GUIEventAdapter::FRAME) {
			if (uav != nullptr) {
				LCreal rpm[4];
				uav->getEngRPM(rpm, 4);
				std::copy_n(std::begin(rpm), 4, mRpms.begin());
				animationRPM(mRpms);
				return true;
			}
		}
		return false;
	}

	void EngineVisitor::setVehicle(Simulation::UnmannedAirVehicle* pav) {
		uav = pav;
	}

	DOFTransform* EngineVisitor::createEngineNode(Node* node, const Vec3& vec) {
		ref_ptr<DOFTransform> transformEngine = new DOFTransform;
		transformEngine->setCurrentTranslate(vec);
		transformEngine->setCurrentScale(Vec3(1, 1, 1));
		transformEngine->addChild(node);
		return transformEngine.release();
	}

	void EngineVisitor::animationRPM(const Rpms& rpm) {
		for (auto i = 0; i < mEngines.size(); ++i)
			mEngines[i]->setIncrementHPR(::osg::Vec3(0.0, 0.0, rpm[i]));
	}
}