#include "UpdateMoveCallback.h"

#include <osg/PositionAttitudeTransform>
#include <osg/Uniform>
#include <JSBSim/math/FGMatrix33.h>
#include <JSBSim/math/FGColumnVector3.h>
#include <iostream>

using namespace osg;
using namespace osgEarth;
using namespace osgEarth::Util;
using namespace JSBSim;

namespace Eaagles {
	UpdateMoveCallback::UpdateMoveCallback( Simulation::AirVehicle* av, osgEarth::Map* map ) : 
		NodeCallback(), 
		Aircraft(av), 
		Map(map), 
		ElevQuery(Map), 
		SRS(SpatialReference::get("epsg:4326")->getECEF()),
		currentRot(0.0,0.0,0.0),
		currentPos(0.0,0.0,0.0),
		currentTerrainElevation(-1.0)	{}

	void UpdateMoveCallback::operator()(Node* node, NodeVisitor* nv) { 
		PositionAttitudeTransform* mt = dynamic_cast<PositionAttitudeTransform*>( node );
		if ( mt != NULL && Aircraft != NULL ) {
			currentRot = Aircraft->getGeocEulerAngles();
			currentPos = Aircraft->getGeocPosition();
		  
			double x = currentPos.x();
			double y = currentPos.y();
			double z = currentPos.z();

			double acRoll = currentRot.x();
			double acPitch = currentRot.y();
			double acYaw = currentRot.z();

			Vec3d Pos(x,y,z);
			osgEarth::GeoPoint GP(SRS, Pos);
			bool foundElevation = ElevQuery.getElevation(GP, currentTerrainElevation);
			currentTerrainElevation /= 0.3048;
			Aircraft->setTerrainElevation(currentTerrainElevation);
			
			Quat Rot(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);
			
			Matrixd Permute(0,  1,  0,  0,
											0,  0, -1,  0,
										 -1,  0,  0,  0,
											0,  0,  0,  1); 

			Matrixd Orient(Rot);
			Orient = Permute * Orient;
			mt->setAttitude( Orient.getRotate() );
			mt->setPosition( Pos );
		}
		traverse(node,nv);
	}
}