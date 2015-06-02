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
			UpdateCallbackCessna( osgViewer::Viewer* Viewer ) :
				NodeCallback(), 
				i(0), 
				translate(), 
				rotate(),
				sceneViewer(Viewer) {}
			
			UpdateCallbackCessna( const UpdateCallbackCessna& copy, const CopyOp& copyop=CopyOp::SHALLOW_COPY ) : 
				NodeCallback(copy, copyop), 
				i(copy.i), 
				translate(copy.translate),
				rotate(copy.translate),
			  sceneViewer(copy.sceneViewer)	{}

		private:
			virtual void operator()(Node* node, NodeVisitor* nv) { 
				MatrixTransform* mt = dynamic_cast<MatrixTransform*>( node );
				if ( mt != NULL ) {
					float acYaw = 0.0f;
					float acPitch = 0.0f;
					float acRoll = 0.0f;

					float xi = 2000.0f;//-20.0f * Basic::Distance::KM2M;
					float yi = 1000.0f;//-20.0f * Basic::Distance::KM2M;
					float zi = 0.0f;
				
					Matrix viewMatrix(
						1, 0, 0, 0,
						0, 0,-1, 0,
						0, 1, 0, 0,
						0, 0, 0, 1);
					rotate = Matrix::rotate(acRoll, X_AXIS, acPitch, Y_AXIS, acYaw, Z_AXIS);
					translate = Matrix::translate(xi, yi, zi);
					
					Matrixd viewPosMatrix = rotate * translate;

					// Invert Model View Matrix!!
					viewPosMatrix.invert(viewPosMatrix);
					// Create New EYE Matrix!!
					Matrix mat = viewPosMatrix * viewMatrix;
					
					sceneViewer->getCamera()->setViewMatrix(mat);
					
					mt->setMatrix( rotate * translate );
				}
				traverse(node,nv);
				return;
		}

		unsigned int i;
		Matrix translate;
		Matrix rotate;
		ref_ptr<osgViewer::Viewer> sceneViewer;
	};
}
#endif