#ifndef ENGINEVISITOR
#define ENGINEVISITOR

#include <osgViewer/ViewerEventHandlers>
#include <osgSim/DOFTransform>
#include <osgGA/GUIEventHandler>
#include <array>

#include <openeaagles/simulation/AirVehicle.h>

namespace Eaagles {

	class EngineVisitor : public ::osg::NodeVisitor, public osgGA::GUIEventHandler {
	public:
		typedef std::array<double, 4> Rpms;
		typedef std::array<Basic::safe_ptr<osgSim::DOFTransform>, 4> Engines;

		EngineVisitor();
		virtual ~EngineVisitor();

		virtual void apply(::osg::Transform& node);
		
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void setVehicle(Simulation::UnmannedAirVehicle* pav);

		static osgSim::DOFTransform* createEngineNode(::osg::Node* node, const ::osg::Vec3& vec);

	protected:
		void animationRPM(const Rpms& rpms);

	private:
		Basic::safe_ptr<Simulation::UnmannedAirVehicle> uav;
		Engines mEngines;
		Rpms mRpms;
	};
}
#endif
