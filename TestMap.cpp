#include "TestMap.h"
#include "UpdateMoveCallback.h"
#include <algorithm>
#include <memory>

using namespace osg;
using namespace osgGA;
using namespace osgSim;
using namespace osgEarth;
using namespace osgEarth::Util;
using namespace std;

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)

	BEGIN_EVENT_HANDLER(TestMap)
		ON_EVENT(RESET_EVENT,onEntry)
	END_EVENT_HANDLER()

	TestMap::TestMap() {
		STANDARD_CONSTRUCTOR()
		int argc = 3;
		char* argv[3] = { {"c:/Users/Fete/Documents/Visual Studio 2012/Projects/demoSubDisplays/subdisplays/Debug/subdisplays.exe"}, {"c:/osgEarth/tests/boston.earth"}, {"--sky"}};

		ArgumentParser arguments(&argc,argv);
		
		int left, top, right, bottom = { 0 };

		//Ugly hack for Windows only!
		RECT DesktopClientRect;
		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DesktopClientRect, 0)) {
			left = DesktopClientRect.left;
			top = DesktopClientRect.top;
			right = DesktopClientRect.right;
			bottom = DesktopClientRect.bottom;
		}
		//
    
		viewer = new osgViewer::Viewer(arguments);
		//Get these numbers from .epp file!
		window = viewer->setUpViewerAsEmbeddedInWindow(300,0,500,600);
    viewer->getDatabasePager()->setUnrefImageDataAfterApplyPolicy( false, false );
				
		engineVisitor = new EngineVisitor;
		viewer->addEventHandler(engineVisitor);
		
		nodeRoot = MapNodeHelper().load( arguments, viewer.get() );
		if( !nodeRoot )
			return;

		nodeMap = MapNode::get( nodeRoot.get() );
		if( !nodeMap )
			return;

		const std::string pathToModels("c:/OpenEaaglesExamples/shared/data/JSBSim/aircraft/copter/models/");

		nodeAircraft = osgDB::readNodeFile(pathToModels + "copter.ac");
		if ( !nodeAircraft )
			return;
		
		nodeEngineCW = osgDB::readNodeFile(pathToModels + "engineCW.ac");
		if( !nodeEngineCW )
			return;

		nodeEngineCCW = osgDB::readNodeFile(pathToModels + "engineCCW.ac");
		if( !nodeEngineCCW )
			return;

		groupAircraft = new Group;
		groupAircraft->addChild( nodeAircraft.get() );

		setupForwardEngine = EngineVisitor::createEngineNode( nodeEngineCCW.get(), Vec3( 0.0, -0.01, -0.2855 ) );
		setupRightEngine = EngineVisitor::createEngineNode( nodeEngineCW.get(), Vec3( 0.2855, -0.01, 0.0 ) );
		setupBackEngine = EngineVisitor::createEngineNode( nodeEngineCCW.get(), Vec3( 0.0, -0.01, 0.2855 ) );
		setupLeftEngine = EngineVisitor::createEngineNode( nodeEngineCW.get(), Vec3( -0.2855, -0.01, 0.0) );
				
		groupAircraft->addChild( setupForwardEngine.get() );
		groupAircraft->addChild( setupRightEngine.get() );
		groupAircraft->addChild( setupBackEngine.get() );
		groupAircraft->addChild( setupLeftEngine.get() );

		nodeModifiedAircraft = new PositionAttitudeTransform;
		nodeModifiedAircraft->addChild( groupAircraft.get() );
				
		nodeRoot->addChild( nodeModifiedAircraft.get() );
		
		nodeTracker = new osgGA::NodeTrackerManipulator;
		//nodeTracker->setHomePosition( Vec3(0, 0, 6), Vec3(), Vec3(0, 1, 0) );
		//nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setHomePosition( Vec3(-6, 0, 0), Vec3(), Vec3(0, 0, 1) );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER );
		nodeTracker->setTrackNode( nodeAircraft.get() );
		viewer->setCameraManipulator( nodeTracker.get() );
		
		light = new Light;
		light->setLightNum( 1 );
		lightSource = new LightSource;
		lightSource->setLight( light.get() );
		nodeRoot->getOrCreateStateSet()->setMode( GL_LIGHT1, StateAttribute::ON );
		nodeRoot->addChild( lightSource.get() );
		
		nodeRoot->accept(*engineVisitor);

		viewer->setSceneData( nodeRoot.get() );
		viewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
		viewer->getCamera()->addCullCallback( new AutoClipPlaneCullCallback(NULL) );
				
		viewer->realize();
	}
	
	bool TestMap::onEntry() {
		SimStation* const sta = static_cast<SimStation*>( findContainerByType(typeid(SimStation)) );
		if (sta != 0) {
      sim = sta->getSimulation();
      av = dynamic_cast<Simulation::AirVehicle*>(sta->getOwnship());
			engineVisitor->setAirVehicle(av);
			nodeModifiedAircraft->setUpdateCallback( new UpdateMoveCallback ( av, nodeMap->getMap() ) );
			double initLat = av->getInitLatitude();
			double initLon = av->getInitLongitude();
			double initHAE = 0.0; // to get just X,Y on the sea level;
			double x, y, z;
			EllipsoidModel EarthEllipsoid;
			EarthEllipsoid.convertLatLongHeightToXYZ(DegreesToRadians(initLat), DegreesToRadians(initLon), initHAE, x, y, z);
			const osgEarth::SpatialReference* SR(SpatialReference::get("epsg:4326")->getECEF());
			osgEarth::GeoPoint GP(SR, x, y, z, osgEarth::ALTMODE_ABSOLUTE);
			double initTerrainElevation = -1;
			osgEarth::Util::ElevationQuery ElevQuery(nodeMap->getMap());
			bool foundTerrainElevation = ElevQuery.getElevation(GP, initTerrainElevation);
			if( foundTerrainElevation && initTerrainElevation != -1 ) {
				av->setAltitude(initTerrainElevation);
			}
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

	void TestMap::reshapeIt(int w, int h) {
		if (window.valid()) {
			window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
			window->getEventQueue()->windowResize(window->getTraits()->x, window->getTraits()->y, w, h);
		}
	}
} // end of Eaagles namespace
