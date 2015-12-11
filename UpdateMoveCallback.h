#ifndef UPDATEMOVECALLBACK
#define UPDATEMOVECALLBACK

#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/Matrix>
#include <osg/Quat>
#include <osg/Vec3d>

#include <openeaagles/basic/Component.h>
#include <openeaagles/simulation/AirVehicle.h>
#include <openeaagles/simulation/Simulation.h>

namespace Eaagles {
	class UpdateMoveCallback : public ::osg::NodeCallback {
	public:
		UpdateMoveCallback( Simulation::AirVehicle* av );
	private:
		virtual void operator()(::osg::Node* node, ::osg::NodeVisitor* nv);
		Basic::safe_ptr<Simulation::AirVehicle> Aircraft;
		osg::Vec3d currentRot;
		osg::Vec3d currentPos;
	};
}
#endif