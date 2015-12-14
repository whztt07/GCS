#ifndef ENGINEBOARD
#define ENGINEBOARD

#include "Engine.h"

#include <osg/NodeVisitor>
#include <osg/Transform>

#include <array>

#include <openeaagles/simulation/AirVehicle.h>

namespace Eaagles {

	class EngineBoard : public ::osg::NodeVisitor {
	public:
		EngineBoard(Simulation::UnmannedAirVehicle* pav);

		virtual void apply(::osg::Transform& node);

		static Engine* createEngineNode(::osg::Node* node, const ::osg::Vec3& vec);

	private:
		Basic::safe_ptr<Simulation::UnmannedAirVehicle> uav;
	};
}
#endif