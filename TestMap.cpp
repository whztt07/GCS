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

		nodeAircraft = dynamic_cast<Group*> (osgDB::readNodeFile("c:/jsbsim/aircraft/copter/models/copter.ac"));
		if ( !nodeAircraft )
			return;
		
		nodeEngineCW = osgDB::readNodeFile("c:/jsbsim/aircraft/copter/models/engineCW.ac");
		if( !nodeEngineCW )
			return;

		nodeEngineCCW = osgDB::readNodeFile("c:/jsbsim/aircraft/copter/models/engineCCW.ac");
		if( !nodeEngineCCW )
			return;

		groupAircraft = new Group;

		ref_ptr<MatrixTransform> setupForwardEngine = new MatrixTransform;
	  setupForwardEngine->setMatrix( Matrix::translate( 0.0, 0.0, -0.2855 ) );
		setupForwardEngine->addChild( nodeEngineCW.get() );
		
		ref_ptr<MatrixTransform> setupRightEngine = new MatrixTransform;
	  setupRightEngine->setMatrix( Matrix::translate( 0.2855, 0.0, 0.0 ) );
		setupRightEngine->addChild( nodeEngineCCW.get() );

		ref_ptr<MatrixTransform> setupBackEngine = new MatrixTransform;
	  setupBackEngine->setMatrix( Matrix::translate( 0.0, 0.0, 0.2855 ) );
		setupBackEngine->addChild( nodeEngineCW.get() );

		ref_ptr<MatrixTransform> setupLeftEngine = new MatrixTransform;
	  setupLeftEngine->setMatrix( Matrix::translate( -0.2855, 0.0, 0.0 ) );
		setupLeftEngine->addChild( nodeEngineCCW.get() );

		groupAircraft->addChild( nodeAircraft.get() );
		groupAircraft->addChild( setupForwardEngine.get() );
		groupAircraft->addChild( setupRightEngine.get() );
		groupAircraft->addChild( setupBackEngine.get() );
		groupAircraft->addChild( setupLeftEngine.get() );

		nodeModifiedAircraft = new PositionAttitudeTransform;
		nodeModifiedAircraft->addChild( groupAircraft.get() );
				
		nodeRoot->addChild( nodeModifiedAircraft.get() );
		
		nodeTracker = new osgGA::NodeTrackerManipulator;
		nodeTracker->setHomePosition( Vec3(0, 0, 6), Vec3(), Vec3(0, 1, 0) );
		nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
		nodeTracker->setTrackNode( nodeAircraft.get() );
		viewer->setCameraManipulator( nodeTracker.get() );
		
		light = new Light;
		light->setLightNum( 1 );
		lightSource = new LightSource;
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
