#ifndef FOLLOWMOVEUPDATER
#define FOLLOWMOVEUPDATER

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Transform>
#include <osgGA/OrbitManipulator>
#include <openeaagles/basic/Component.h>

using namespace osg;

namespace Eaagles {
	class FollowUpdater : public osgGA::GUIEventHandler {
	public:
		FollowUpdater( Node* node ) : _target(node) {}
		virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
			osgViewer::View* view = static_cast<osgViewer::View*>(&aa);
			if ( !view || !_target || ea.getEventType()!=osgGA::GUIEventAdapter::FRAME ) 
				return false;
			osgGA::OrbitManipulator* orbit = dynamic_cast<osgGA::OrbitManipulator*>( view->getCameraManipulator() );
			if ( orbit ) {
				Matrix matrix = computeTargetToWorldMatrix( _target.get() );
				Vec3d targetCenter = _target->getBound().center() * matrix;
				orbit->setCenter( targetCenter );
			}
			return false;
		}
		Matrix computeTargetToWorldMatrix( Node* node ) const {
			Matrix l2w;
			if ( node && node->getNumParents() > 0 ) {
				Group* parent = node->getParent(0);
				l2w = computeLocalToWorld( parent->getParentalNodePaths()[0] );
			}
			return l2w;
		}
	protected:
		observer_ptr<Node> _target;
	};
}

#endif