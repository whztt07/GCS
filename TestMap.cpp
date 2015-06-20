#include "TestMap.h"
#include "UpdateMoveCallback.h"
#include <osg/PositionAttitudeTransform>

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		
		const double centerLat = 30.5293056;
		const double centerLon = -85.4879167;
		const double cessnaHeight = 200.0;

		rootnode = new Group;
		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessnafire.osg");
		map = osgDB::readNodeFile("c:/Terrain/FromUSGS/output/out.osgb");
		camera = new Camera;
		viewer = new osgViewer::Viewer;
		Vec3d center, eye, up;
		
		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessnafire.osg");
		if ( !cessna )
			return;

		map = osgDB::readNodeFile("c:/Terrain/FromUSGS/output/out.osgb");
		if (!map )
			return;
				
		//Getting XYZ position for cessna
		Matrix cessnaLocation;
		EllipsoidModel ellipsoid;
		double x,y,z;
		ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), cessnaHeight, x, y, z);
		Vec3 positionForCessna = Vec3d(x,y,z);

		//Placing cessna
		moveCessna = new PositionAttitudeTransform;
		moveCessna->setPosition(positionForCessna);
		//Calculating attitude (heading north)
		double phi = 0.0;
		double psi = 0.0;
		double theta = 0.0;
		Matrixd localToWorld;
		Matrixd attitude;
		ellipsoid.computeLocalToWorldTransformFromXYZ(DegreesToRadians(centerLat), DegreesToRadians(centerLon), cessnaHeight, localToWorld);
		attitude.makeRotate(DegreesToRadians(phi), Y_AXIS, DegreesToRadians(theta), X_AXIS, DegreesToRadians(-psi), Z_AXIS);
		attitude *= localToWorld;
		Quat quat = attitude.getRotate();
		moveCessna->setAttitude(quat);
		moveCessna->setUpdateCallback( new UpdateCallbackCessna );
		moveCessna->addChild(cessna.get());
				
		// Create camera as shallow copy of theo ne of the view
		camera = dynamic_cast<Camera*>(viewer->getCamera()->clone(CopyOp::SHALLOW_COPY));
		camera->setProjectionMatrixAsPerspective(500.0,1.33,0.1,10000.0);
		camera->setCullingActive(false);

		//Getting XYZ position for camera
		//Lat Lon are the same, height is 500.0
		// The eye : position of the camera
		ellipsoid.convertLatLongHeightToXYZ(DegreesToRadians(centerLat+0.000056), DegreesToRadians(centerLon-0.000056), 320.0, x, y, z);
		eye = Vec3d(x,y,z);
		// The center : position where you look at same position a little bit underneath...
		//ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), 299.9, x, y, z);
		center = positionForCessna;//osg::Vec3d(x,y,z);
		// The up : a little more tricky...
		// It is the up vector of your screen (ie what is the bottom top axis of your screen)
		// If you want it to be north up
		up = Vec3d ( -std::cos(DegreesToRadians(centerLat)) * std::sin( DegreesToRadians(centerLon)), 
			           -std::sin(DegreesToRadians(centerLat)) * std::sin(DegreesToRadians(centerLon)), 
			            std::cos(DegreesToRadians(centerLon)));
		// If you want it to be east up
		//up = osg::Vec3d ( -std::cos(osg::DegreesToRadians(centerLat)), std::cos(osg::DegreesToRadians(centerLon)), 0.0);
		//up = osg::Vec3d ( 0.0, 0.1, 0.0);
		// Now you can set your view matrix
		camera->setViewMatrixAsLookAt(eye,center,up);
		viewer->setCamera( camera.get() );

		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(-50, 50.0, 150), Vec3(), Z_AXIS );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( cessna.get() );
		
		rootnode->addChild( map.get() );
		rootnode->addChild( moveCessna.get() );

		viewer->setUpViewerAsEmbeddedInWindow(0,0,1000,1000);
		
		viewer->setCameraManipulator( nodeTracker.get() );
		viewer->setSceneData( rootnode.get() );
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
		if (viewer->isRealized()) 
			viewer->frame();
	}
} // end of Eaagles namespace

