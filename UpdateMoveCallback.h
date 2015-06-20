#ifndef UPDATEMOVECALLBACK
#define UPDATEMOVECALLBACK

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Transform>
#include <openeaagles/basic/Component.h>

using namespace osg;

namespace Eaagles {
	class UpdateCallbackCessna : public NodeCallback {
		public:
			UpdateCallbackCessna() : NodeCallback(), translate(), rotate() {}
			UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : NodeCallback(copy, copyop), translate(copy.translate),	rotate(copy.translate) {}
			virtual ~UpdateCallbackCessna() {}
		private:
			virtual void operator()(Node* node, NodeVisitor* nv) { 
				PositionAttitudeTransform* mt = dynamic_cast<PositionAttitudeTransform*>( node );
				if ( mt != NULL ) {
					float acYaw = 0.0;
					float acPitch = 0.0;
					float acRoll = 0.0;

					Vec3d vec3dPos = mt->getPosition();

					float xi, yi, zi;
					xi = vec3dPos.x();
					yi = vec3dPos.y();
					zi = vec3dPos.z();

					zi = zi + 1;

					rotate = Matrix::rotate(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);
					translate = Matrix::translate(xi, yi, zi);
					mt->setPosition( translate.getTrans() );
					mt->setAttitude( rotate.getRotate() );
				}
				traverse(node,nv);
				return;
		}
		Matrix translate;
		Matrix rotate;
	};
}
#endif