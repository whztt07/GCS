#include "UpdateMoveCallback.h"
#include <osg/PositionAttitudeTransform>
#include <osg/CoordinateSystemNode>

using namespace osg;

namespace Eaagles {
	UpdateCallbackCessna::UpdateCallbackCessna( Simulation::AirVehicle* av ) : NodeCallback(), translate(), rotate(), Aircraft(av) {}
	//UpdateCallbackCessna::UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : NodeCallback(copy, copyop), translate(copy.translate),	rotate(copy.translate) {}

	void getEulerFromQuat(osg::Quat q, double& heading, double& attitude, double&bank) {
		double limit = 0.499999;
		double sqx = q.x()*q.x();
		double sqy = q.y()*q.y();
		double sqz = q.z()*q.z();
		double t = q.x()*q.y() + q.z()*q.w();
		
		if (t>limit) { // gimbal lock ?
			heading = 2 * atan2(q.x(),q.w());
			attitude = osg::PI_2;
			bank = 0;
		}
		else if (t<-limit) {
			heading = -2 * atan2(q.x(),q.w());
			attitude = - osg::PI_2;
			bank = 0;
		}
		else {
			heading = atan2(2*q.y()*q.w()-2*q.x()*q.z() , 1 - 2*sqy - 2*sqz);
			attitude = asin(2*t);
			bank = atan2(2*q.x()*q.w()-2*q.y()*q.z() , 1 - 2*sqx - 2*sqz);
		}
	}

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

			/*
				EllipsoidModel ellipsoid;
				double lat, lon, height;
				ellipsoid.convertXYZToLatLongHeight(x,y,z,lat,lon,height);
				lat = RadiansToDegrees(lat);
				lon = RadiansToDegrees(lon);
			*/
						
			//getEulerFromQuat(currRot, acYaw, acPitch, acRoll);

			Quat Rot(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);

			mt->setAttitude( Rot );
			mt->setPosition( Pos );
		}
		traverse(node,nv);
		return;
	}
}