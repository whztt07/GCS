#include "TestMap.h"
#include "UpdateMoveCallback.h"

using namespace osg;
using namespace osgEarth;
using namespace osgEarth::Util;

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		int argc = 3;
		char* argv[3] = { {"c:/Users/Fete/Documents/Visual Studio 2012/Projects/demoSubDisplays/subdisplays/Debug/subdisplays.exe"}, {"c:/osgEarth/tests/readymap.earth"}, {"--sky"}};

		ArgumentParser arguments(&argc,argv);
				
    viewer = new osgViewer::Viewer(arguments);
		viewer->setUpViewerAsEmbeddedInWindow(0,0,1400,1000);
    viewer->getDatabasePager()->setUnrefImageDataAfterApplyPolicy( false, false );

		nodeRoot = MapNodeHelper().load( arguments, viewer.get() );
		if( !nodeRoot )
			return;

		nodeMap = MapNode::get( nodeRoot.get() );
		if( !nodeMap )
			return;

		nodeAircraft = osgDB::readNodeFile("c:/jsbsim/aircraft/copter/models/copter.ac");
		if ( !nodeAircraft )
			return;
		
		/*
		const double centerLat = 42.3583333;
		const double centerLon = -71.0602778;
		const double AircraftASL = 10000.0;
		
		EllipsoidModel ellipsoid;
		double x,y,z;
		ellipsoid.convertLatLongHeightToXYZ(osg::DegreesToRadians(centerLat), osg::DegreesToRadians(centerLon), AircraftASL, x, y, z);
		Vec3 positionForAircraft = Vec3d(x,y,z);
		*/
		nodeModifiedAircraft = new PositionAttitudeTransform;
		//positionedAircraft->setPosition( positionForAircraft );
		nodeModifiedAircraft->addChild( nodeAircraft.get() );
				
		nodeRoot->addChild( nodeModifiedAircraft.get() );
		
		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(0, 0, 5), Vec3(), Vec3(0, 1, 0) );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( nodeAircraft.get() );
		viewer->setCameraManipulator( nodeTracker.get() );
		
		ref_ptr<Light> light = new Light;
		light->setLightNum( 1 );
		ref_ptr<LightSource> lightSource = new LightSource;
		lightSource->setLight( light.get() );
		nodeRoot->getOrCreateStateSet()->setMode( GL_LIGHT1, StateAttribute::ON );
		
		nodeRoot->addChild( lightSource.get() );
		
		viewer->setSceneData( nodeRoot.get() );
		viewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
		viewer->getCamera()->addCullCallback( new osgEarth::Util::AutoClipPlaneCullCallback(NULL) );
				
		viewer->realize();
	}
	
	bool TestMap::onEntry() {
		SimStation* const sta = static_cast<SimStation*>( findContainerByType(typeid(SimStation)) );
		if (sta != 0) {
      sim = sta->getSimulation();
      av = dynamic_cast<Simulation::AirVehicle*>(sta->getOwnship());
			nodeModifiedAircraft->setUpdateCallback( new UpdateMoveCallback ( av, nodeMap->getMap() ) );
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
