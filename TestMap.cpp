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
		const double cessnaHeight = 1000.0;
		
		rootnode = new Group;
		viewer = new osgViewer::Viewer;
		viewer->setUpViewerAsEmbeddedInWindow(0,0,1400,1000);
		viewer->setSceneData( rootnode.get() );

		map = osgDB::readNodeFile("c:/Terrain/FromUSGS/output/out.osgb");
		if (!map )
			return;
		rootnode->addChild( map.get() );

		cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessna.osg");
		if ( !cessna )
			return;
		EllipsoidModel ellipsoid;
		double x,y,z;
		ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), cessnaHeight, x, y, z);
		Vec3 positionForCessna = Vec3d(x,y,z);
		
		movedCessna = new PositionAttitudeTransform;
		movedCessna->setPosition( positionForCessna );
		movedCessna->addChild( cessna.get() );
		rootnode->addChild( movedCessna.get() );
		
		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(0,90,0), Vec3(), Z_AXIS );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( cessna.get() );
		viewer->setCameraManipulator( nodeTracker.get() );
		
		ref_ptr<Light> light = new Light;
		light->setLightNum( 1 );
		ref_ptr<LightSource> lightSource = new LightSource;
		lightSource->setLight( light.get() ); 
		rootnode->getOrCreateStateSet()->setMode( GL_LIGHT1, StateAttribute::ON );
		rootnode->addChild( lightSource.get() );

		viewer->realize();
	}
	
	bool TestMap::onEntry() {
		SimStation* const sta = static_cast<SimStation*>( findContainerByType(typeid(SimStation)) );
		if (sta != 0) {
      sim = sta->getSimulation();
      av = dynamic_cast<Simulation::AirVehicle*>(sta->getOwnship());
			movedCessna->setUpdateCallback( new UpdateCallbackCessna (av) );
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

