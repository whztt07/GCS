#ifndef ENGINEROTATECALLBACK
#define ENGINEROTATECALLBACK

#include <osg/NodeCallback>

#include <openeaagles/simulation/AirVehicle.h>

namespace Eaagles {

	class EngineRotateCallback : public ::osg::NodeCallback {
	public:
		EngineRotateCallback(Simulation::UnmannedAirVehicle* pav);

		virtual void operator()(::osg::Node* node, ::osg::NodeVisitor* nv);
		
	private:
		static const int NUMBER_OF_ENGINES = 4;
		Basic::safe_ptr<Simulation::UnmannedAirVehicle> uav;
	};
}
#endif
