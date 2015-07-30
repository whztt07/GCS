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

#include <osgEarth/ElevationQuery>
#include <osgEarth/Map>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/Units>

namespace Eaagles {
	class UpdateMoveCallback : public ::osg::NodeCallback {
	public:
		UpdateMoveCallback( Simulation::AirVehicle* av, osgEarth::Map* map );
	private:
		virtual void operator()(::osg::Node* node, ::osg::NodeVisitor* nv);
		Simulation::AirVehicle* Aircraft;
		osgEarth::Map* Map;
		osgEarth::Util::ElevationQuery ElevQuery;
		const osgEarth::SpatialReference* SRS;
		osg::Vec3d currentRot;
		osg::Vec3d currentPos;
		double currentTerrainElevation;
	};
}
#endif