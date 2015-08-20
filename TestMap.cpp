#include "TestMap.h"
#include "UpdateMoveCallback.h"
#include <algorithm>

using namespace osg;
using namespace osgEarth;
using namespace osgEarth::Util;

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
	EMPTY_SERIALIZER(TestMap)
	
	class UpdateCallback : public NodeCallback {
	public:
		UpdateCallback(Simulation::AirVehicle* av, int engineNumber) : Aircraft(av), EngineNumber(engineNumber), angle(0.0), deltaAngle(0.0), rotation() {}
		virtual void operator()(Node* node, NodeVisitor* nv) {
			MatrixTransform* mt = dynamic_cast<MatrixTransform*>( node );
			if ( mt != NULL && Aircraft != NULL ) {
				LCreal rpm[4];
				Aircraft->getEngRPM(rpm, 4);
				LCreal rps[4];
				std::transform(rpm, rpm + (sizeof(rpm) / sizeof(LCreal)), rps,  [](LCreal d) { return d / 60.0f; } );
				deltaAngle = 2.0f * osg::PI / ((float)rps[EngineNumber]);
				if(angle > 2.0f * osg::PI)
					angle = 0.0;
				angle += deltaAngle;
				rotation.makeRotate(angle, Y_AXIS);
				Matrix rotate(rotation);
				Matrix translate = mt->getMatrix();
				Matrix setupTransAndRot = rotate * translate;
				mt->setMatrix(setupTransAndRot);
			}
      traverse(node,nv);
		}
	private:
		Simulation::AirVehicle* Aircraft;
		int EngineNumber;
		double angle;
		double deltaAngle;
		Quat rotation;
	};

	MatrixTransform* createTransformNode( Node* node, const Matrix& matrix ) {
		ref_ptr<MatrixTransform> trans = new MatrixTransform;
		trans->addChild( node );
		trans->setMatrix( matrix );
		return trans.release();
	}
	
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

		setupForwardEngine = createTransformNode( nodeEngineCCW.get(), Matrix::translate( 0.0, -0.01, -0.2855 ) );
		setupRightEngine = createTransformNode( nodeEngineCW.get(), Matrix::translate( 0.2855, -0.01, 0.0 ) );
		setupBackEngine = createTransformNode( nodeEngineCCW.get(), Matrix::translate( 0.0, -0.01, 0.2855 ) );
		setupLeftEngine = createTransformNode( nodeEngineCW.get(), Matrix::translate( -0.2855, -0.01, 0.0) );
				
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
		viewer->getCamera()->addCullCallback( new AutoClipPlaneCullCallback(NULL) );
				
		viewer->realize();
	}
	
	bool TestMap::onEntry() {
		SimStation* const sta = static_cast<SimStation*>( findContainerByType(typeid(SimStation)) );
		if (sta != 0) {
      sim = sta->getSimulation();
      av = dynamic_cast<Simulation::AirVehicle*>(sta->getOwnship());
			setupForwardEngine->setUpdateCallback(new UpdateCallback(av, 0) );
			setupRightEngine->setUpdateCallback(new UpdateCallback(av, 1) );
			setupBackEngine->setUpdateCallback(new UpdateCallback(av, 2) );
			setupLeftEngine->setUpdateCallback(new UpdateCallback(av, 3) );
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
