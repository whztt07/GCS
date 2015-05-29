#ifndef UPDATEMOVECALLBACK
#define UPDATEMOVECALLBACK

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Transform>
#include <string>
#include <vector>

#include <openeaagles/basic/Component.h>

using namespace osg;

namespace Eaagles {
	class UpdateCallbackCessna : public NodeCallback {
		public:
			UpdateCallbackCessna() : NodeCallback(), i(0) {}
			UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : 
				NodeCallback(copy, copyop), 
				i(copy.i), 
				translate(copy.translate),
				rotate(copy.translate) {}
		private:
			virtual void operator()(Node* node, NodeVisitor* nv) { 
				
				float acYaw = 0.0f;
				float acPitch = 0.0f;
				float acRoll = 0.0f;

				float xi = 0.0f;//-20.0f * Basic::Distance::KM2M;
				float yi = 0.0f;//-20.0f * Basic::Distance::KM2M;
				float zi = 0.0f;
				
				MatrixTransform* mt = dynamic_cast<MatrixTransform*>( node );
				if ( mt != NULL ) {
					translate = Matrix::translate(xi, yi, zi);
					rotate = Matrix::rotate(acPitch, Y_AXIS, acRoll, X_AXIS, acYaw, Z_AXIS);
					mt->setMatrix( rotate * translate );
				}
				traverse(node,nv);
				return;
		}
		unsigned int i;
		Matrix translate;
		Matrix rotate;
	};
}
#endif