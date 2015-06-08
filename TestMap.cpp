#include "TestMap.h"
#include "UpdateMoveCallback.h"
#include "FollowMoveUpdater.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		
		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessnafire.osg");
		if ( !cessna )
			return;

		mtMove = new MatrixTransform;
		mtMove->setUpdateCallback( new UpdateCallbackCessna );
		mtMove->addChild( cessna.get() );
		
		terrain = new MatrixTransform;
		terrain->addChild( osgDB::readNodeFile("c:/Terrain/out.osgb") );
		terrain->setMatrix( Matrix::translate(0.0f, 0.0f, 0.0f) );

		rootnode = new Group;
		rootnode->addChild( mtMove.get() );
		rootnode->addChild( terrain.get() );

		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(-50, -100.0, 150), Vec3(), Z_AXIS );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( cessna.get() );
		
		viewer = new osgViewer::Viewer;
		viewer->setUpViewerAsEmbeddedInWindow(0,0,1000,1000);
		viewer->setCameraManipulator( nodeTracker.get() );
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
	
	void TestMap::drawFunc(void) {
		// update and render the scene graph
		if (viewer->isRealized()) 
			viewer->frame();
	  // Swap Buffers
	  glutSwapBuffers();
	  glutPostRedisplay();
		BaseClass::drawFunc();
	}
} // end of Eaagles namespace

