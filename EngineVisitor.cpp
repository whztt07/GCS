#include "EngineVisitor.h"

using namespace osg;
using namespace osgSim;
using namespace osgGA;

namespace Eaagles {

	EngineVisitor::EngineVisitor() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN), av(NULL) {}

	EngineVisitor::~EngineVisitor() {}

	void EngineVisitor::apply(Transform& node) {
		DOFTransform* pDof = dynamic_cast<DOFTransform*>(&node);
		if (pDof) {
			mDofs.push_back(pDof);
			pDof->setAnimationOn(true);
			pDof->setIncrementHPR(Vec3(0.0, 0.0, 0.0));
			pDof->setMaxHPR(Vec3(0.0, 0.0, 45000.0));
			pDof->setMinHPR(Vec3(0.0, 0.0, 0.0));
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
			if (av != NULL) {
				LCreal rpm[4];
				av->getEngRPM(rpm, 4);
				animationRPM(rpm[0]);
				return true;
			}
		}
		return false;
	}

	void EngineVisitor::setAirVehicle(Simulation::AirVehicle* pav) {
		av = pav;
	}

	DOFTransform* EngineVisitor::createEngineNode(Node* node, const Vec3& vec) {
		ref_ptr<DOFTransform> transformEngine = new DOFTransform;
		transformEngine->setCurrentTranslate(vec);
		transformEngine->setCurrentScale(Vec3(1, 1, 1));
		transformEngine->addChild(node);
		return transformEngine.release();
	}

	void EngineVisitor::animationRPM(double rpm) {
		for (size_t i = 0; i < mDofs.size(); ++i)
			mDofs[i]->setIncrementHPR(::osg::Vec3(0.0, 0.0, rpm));
	}
}