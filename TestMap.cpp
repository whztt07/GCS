#include "TestMap.h"
#include "UpdateMoveCallback.h"

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
				l2w = computeLocalToWorld( parent-> getParentalNodePaths()[0] );
			}
			return l2w;
		}
	protected:
		observer_ptr<Node> _target;
	};
	
	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		
		rootnode = new Group;

		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessnafire.osg");
		cessna->setDataVariance( ::osg::Object::STATIC );
		mtMove = new MatrixTransform;
		mtMove->setDataVariance( ::osg::Object::STATIC );
		mtMove->addChild( cessna.get() );
		mtMove->setUpdateCallback( new UpdateCallbackCessna );
		rootnode->addChild( mtMove.get() );

		map = osgDB::readNodeFile("c:/Terrain/out.osgb");
		rootnode->addChild( map.get() );
		
		viewer = new osgViewer::Viewer;
		window = viewer->setUpViewerAsEmbeddedInWindow(0,0,1000,1000);
		//viewer->addEventHandler(new FollowUpdater(cessna.get()) );
		viewer->setCameraManipulator(new osgGA::TrackballManipulator);
		viewer->addEventHandler(new osgViewer::StatsHandler);
		viewer->setSceneData(rootnode.get());
		
		viewer->realize();
	}
		
	void TestMap::copyData(const TestMap& org, const bool) {
		BaseClass::copyData(org);
	}
	
	EMPTY_DELETEDATA(TestMap)
	
	void TestMap::updateData(const LCreal dt) {
		BaseClass::updateData(dt);
	}
	
	void TestMap::draw() {
		// update and render the scene graph
	  if (viewer.valid()) 
			viewer->frame();
	  // Swap Buffers
	  glutSwapBuffers();
	  glutPostRedisplay();
		BaseClass::draw();
	}
} // end of Eaagles namespace

