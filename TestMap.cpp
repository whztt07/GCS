#include "TestMap.h"
#include "UpdateMoveCallback.h"

using namespace osg;

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		
		const double centerLat = 30.5293056;
		const double centerLon = -85.4879167;
		const double cessnaHeight = 300.0;

		rootnode = new Group;
		//camera = new Camera;
		viewer = new osgViewer::Viewer;
		Vec3d center, eye, up;
		
		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessna.osg");
		if ( !cessna )
			return;

		map = osgDB::readNodeFile("c:/Terrain/FromUSGS/output/out2.osgb");
		if (!map )
			return;
		
		EllipsoidModel ellipsoid;

		rotateCessna = new PositionAttitudeTransform;
		double phi = 0.0;
		double psi = 0.0;
		double theta = 0.0;
		Matrixd localToWorld;
		Matrixd attitude;
		//ellipsoid.computeLocalToWorldTransformFromXYZ(DegreesToRadians(centerLat), DegreesToRadians(centerLon), cessnaHeight, localToWorld);
		attitude.makeRotate(DegreesToRadians(phi), X_AXIS, DegreesToRadians(theta), Y_AXIS, DegreesToRadians(psi), Z_AXIS);
		//attitude *= localToWorld;
		Quat quat = attitude.getRotate();
		//rotateCessna->setAttitude(quat);
		rotateCessna->addChild(cessna.get());

		//Getting XYZ position for cessna
		Matrix cessnaLocation;
		double x,y,z;
		ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), cessnaHeight, x, y, z);
		Vec3 positionForCessna = Vec3d(x,y,z);
		
		//Placing cessna
		moveCessna = new PositionAttitudeTransform;
		moveCessna->setPosition(positionForCessna);
		moveCessna->addChild(rotateCessna.get());
		
		// Create camera as shallow copy of theo ne of the view
		//camera = dynamic_cast<Camera*>(viewer->getCamera()->clone(CopyOp::SHALLOW_COPY));
		//camera->setProjectionMatrixAsPerspective(50.0,1.33,0.1,1000.0);
		//camera->setCullingActive(false);

		//Getting XYZ position for camera
		//Lat Lon are the same, height is 500.0
		// The eye : position of the camera
		//ellipsoid.convertLatLongHeightToXYZ(DegreesToRadians(centerLat+0.000056), DegreesToRadians(centerLon-0.000056), 200.0, x, y, z);
		//eye = Vec3d(x,y,z);
		// The center : position where you look at same position a little bit underneath...
		//ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), 299.9, x, y, z);
		//center = positionForCessna;//osg::Vec3d(x,y,z);
		// The up : a little more tricky...
		// It is the up vector of your screen (ie what is the bottom top axis of your screen)
		// If you want it to be north up
		//up = Vec3d ( -std::cos(DegreesToRadians(centerLat)) * std::sin(DegreesToRadians(centerLon)), 
			           //-std::sin(DegreesToRadians(centerLat)) * std::sin(DegreesToRadians(centerLon)), 
			            //std::cos(DegreesToRadians(centerLon)));
		// If you want it to be east up
		//up = Vec3d ( -std::cos(DegreesToRadians(centerLat)), std::cos(DegreesToRadians(centerLon)), 0.0);
		// Now you can set your view matrix
		//camera->setViewMatrixAsLookAt(eye,center,up);
		//viewer->setCamera( camera.get() );

		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(0,90,0), Vec3(), Z_AXIS );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( cessna.get() );
		
		rootnode->addChild( map.get() );
		rootnode->addChild( moveCessna.get() );
		
		viewer->setUpViewerAsEmbeddedInWindow(0,0,1400,1000);
		
		viewer->setCameraManipulator( nodeTracker.get() );
		viewer->setSceneData( rootnode.get() );
		viewer->realize();
	}
	
	bool TestMap::onEntry() {
		SimStation* const sta = static_cast<SimStation*>( findContainerByType(typeid(SimStation)) );
		if (sta != 0) {
      sim = sta->getSimulation();
      av = dynamic_cast<Simulation::AirVehicle*>(sta->getOwnship());
			moveCessna->setUpdateCallback( new UpdateCallbackCessna (av) );
			return true;
		}
		return false;
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

