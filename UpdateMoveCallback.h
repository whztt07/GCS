#ifndef UPDATEMOVECALLBACK
#define UPDATEMOVECALLBACK

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Transform>
#include <osg/Quat>
#include <osg/Vec3d>

#include <openeaagles/basic/Component.h>
#include <openeaagles/simulation/AirVehicle.h>
#include <openeaagles/simulation/Simulation.h>

namespace Eaagles {
	class UpdateCallbackCessna : public ::osg::NodeCallback {
	public:
		UpdateCallbackCessna( Simulation::AirVehicle* av );
		//UpdateCallbackCessna( const UpdateCallbackCessna& copy, const ::osg::CopyOp& copyop=::osg::CopyOp::SHALLOW_COPY );
	private:
		virtual void operator()(::osg::Node* node, ::osg::NodeVisitor* nv);
		::osg::Matrix translate;
		::osg::Matrix rotate;
		Simulation::AirVehicle* Aircraft;
	};
}
#endif