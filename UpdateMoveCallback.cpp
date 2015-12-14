#include "UpdateMoveCallback.h"

using namespace osg;

namespace Eaagles {
	UpdateMoveCallback::UpdateMoveCallback( Simulation::UnmannedAirVehicle* av ) : 
		NodeCallback(), 
		Aircraft(av), 
		currentRot(0.0,0.0,0.0),
		currentPos(0.0,0.0,0.0)	{}

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