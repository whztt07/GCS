#include "UpdateMoveCallback.h"
#include <osg/PositionAttitudeTransform>
//#include <osg/CoordinateSystemNode>

using namespace osg;

namespace Eaagles {
	UpdateCallbackCessna::UpdateCallbackCessna( Simulation::AirVehicle* av ) : NodeCallback(), translate(), rotate(), Aircraft(av) {}
	//UpdateCallbackCessna::UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : NodeCallback(copy, copyop), translate(copy.translate),	rotate(copy.translate) {}

	void UpdateCallbackCessna::operator()(Node* node, NodeVisitor* nv) { 
		PositionAttitudeTransform* mt = dynamic_cast<PositionAttitudeTransform*>( node );
		if ( mt != NULL && Aircraft != NULL ) {
			const osg::Vec3d currRot = Aircraft->getGeocEulerAngles();
			const osg::Vec3d currPos = Aircraft->getGeocPosition();

			double x = currPos.x();
			double y = currPos.y();
			double z = currPos.z();

			double acRoll = currRot.x();
			double acPitch = currRot.y();
			double acYaw = currRot.z();

			Vec3d Pos(x,y,z);
						
			Quat Rot(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);

			Matrixd Permute(0, -1, 0, 0,
											-1, 0, 0, 0,
											0, 0, -1, 0,
											0, 0, 0, 1); 
		
			Matrixd Orient(Rot);
			Orient = Permute * Orient;
			Quat D(Orient.getRotate());
			mt->setAttitude( D );
			mt->setPosition( Pos );
		}
		traverse(node,nv);
		return;
	}
}