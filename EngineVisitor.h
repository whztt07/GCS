#ifndef ENGINEVISITOR_H
#define ENGINEVISITOR_H

#include <osgViewer/ViewerEventHandlers>
#include <osgSim/DOFTransform>
#include <osgGA/GUIEventHandler>
#include <vector>

#include <openeaagles/simulation/AirVehicle.h>

namespace Eaagles {

	class EngineVisitor : public ::osg::NodeVisitor, public osgGA::GUIEventHandler {
	public:
		EngineVisitor() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN), av(NULL) {}
		virtual ~EngineVisitor() {}

		virtual void apply(::osg::Transform& node) {
			osgSim::DOFTransform* pDof = dynamic_cast<osgSim::DOFTransform*>(&node);
			if (pDof) {
				mDofs.push_back(pDof);
				pDof->setAnimationOn(true);
				pDof->setIncrementHPR(::osg::Vec3(0.0, 0.0, 0.0));
				pDof->setMaxHPR(::osg::Vec3(0.0, 0.0, 45000.0));
				pDof->setMinHPR(::osg::Vec3(0.0, 0.0, 0.0));
			}
			NodeVisitor::apply(node);
		}

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
			osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
			if (!viewer)
				return false;

			if (ea.getHandled())
				return false;

			if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME) {
				if (av != NULL) {
					LCreal rpm[4];
					av->getEngRPM(rpm, 4);
					animationRPM(rpm[0]);
					return true;
				}
			}
			return false;
		}

		void setAirVehicle(Simulation::AirVehicle* pav) {
			av = pav;
		}

		static osgSim::DOFTransform* createEngineNode(::osg::Node* node, const ::osg::Vec3& vec) {
			::osg::ref_ptr<::osgSim::DOFTransform> transformEngine = new osgSim::DOFTransform;
			transformEngine->setCurrentTranslate(vec);
			transformEngine->setCurrentScale(::osg::Vec3(1, 1, 1));
			transformEngine->addChild(node);
			return transformEngine.release();
		}

	private:
		void animationRPM(double rpm) {
			for (size_t i = 0; i < mDofs.size(); ++i)
				mDofs[i]->setIncrementHPR(::osg::Vec3(0.0, 0.0, rpm));
		}
		Simulation::AirVehicle* av;
		std::vector<osgSim::DOFTransform*> mDofs;
	};
}
#endif
