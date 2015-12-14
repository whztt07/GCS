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
		EngineBoard();

		virtual void apply(::osg::Transform& node);

		static Engine* createEngineNode(::osg::Node* node, const ::osg::Vec3& vec);

		void setupAircraft(Simulation::UnmannedAirVehicle* pav);

	private:
		static const int NUMBER_OF_ENGINES = 4;

		typedef std::array<double, NUMBER_OF_ENGINES> Rpms;
		typedef std::array<Basic::safe_ptr<Engine>, NUMBER_OF_ENGINES> Engines;

		Engines mEngines;
		Rpms mRpms;
		Basic::safe_ptr<Simulation::UnmannedAirVehicle> uav;
	};
}
#endif