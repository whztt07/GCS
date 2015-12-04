#ifndef ENGINEVISITOR
#define ENGINEVISITOR

#include <osgViewer/ViewerEventHandlers>
#include <osgSim/DOFTransform>
#include <osgGA/GUIEventHandler>
#include <vector>

#include <openeaagles/simulation/AirVehicle.h>

namespace Eaagles {

	class EngineVisitor : public ::osg::NodeVisitor, public osgGA::GUIEventHandler {
	public:
		EngineVisitor();
		virtual ~EngineVisitor();

		virtual void apply(::osg::Transform& node);
		
		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void setAirVehicle(Simulation::AirVehicle* pav);

		static osgSim::DOFTransform* createEngineNode(::osg::Node* node, const ::osg::Vec3& vec);

	private:
		void animationRPM(double rpm);

		Simulation::AirVehicle* av;
		std::vector<osgSim::DOFTransform*> mDofs;
	};
}
#endif
