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
			UpdateCallbackCessna() : NodeCallback(), i(0), translate(), rotate() {}
			UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : NodeCallback(copy, copyop), i(copy.i), translate(copy.translate),	rotate(copy.translate) {}
			virtual ~UpdateCallbackCessna() {}
		private:
			virtual void operator()(Node* node, NodeVisitor* nv) { 
				MatrixTransform* mt = dynamic_cast<MatrixTransform*>( node );
				if ( mt != NULL ) {
					float acYaw = 0.0f;
					float acPitch = 0.0f;
					float acRoll = 0.0f;

					float xi = i;
					float yi = i;
					float zi = i;
				
					rotate = Matrix::rotate(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);
					translate = Matrix::translate(xi, yi, zi);
					mt->setMatrix( rotate * translate );
					i++;
				}
				traverse(node,nv);
				return;
		}
		int i;
		Matrix translate;
		Matrix rotate;
	};
}
#endif