//------------------------------------------------------------------------------
// Class: Map
//
// Description: Displays 3D map of the environment and the air vehicle.
//------------------------------------------------------------------------------
#ifndef MAP
#define MAP

#include "SimStation.h"
#include "EngineVisitor.h"

#include <osgViewer/Viewer>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/LightSource>
#include <osg/ArgumentParser>

#include <osgEarth/Notify>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/Units>
#include <osgEarth/Viewpoint>
#include <osgEarth/ElevationQuery>
#include <osgEarth/Map>

#include <osgSim/DOFTransform>

#include <openeaagles/basic/Pair.h>
#include <openeaagles/basic/Timers.h>
#include <openeaagles/basic/Parser.h>
#include <openeaagles/basicGL/Graphic.h>
#include <openeaagles/simulation/AirVehicle.h>
#include <openeaagles/simulation/Simulation.h>
#include <openeaagles/gui/glut/GlutDisplay.h>
#include <openeaagles/gui/glut/Factory.h>
#include <openeaagles/basic/Factory.h>
#include <openeaagles/basicGL/Factory.h>
#include <openeaagles/instruments/Factory.h>

namespace Eaagles {
	
	class Map : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(Map, Glut::GlutDisplay)
	public:
		Map();

    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
		
		//BasicGL::Page interface
		virtual void draw();
		virtual bool onEntry();

		//Basic::Component interface
		virtual bool event(const int event, Basic::Object* const obj = nullptr) override;
		
		//BasicGL::Display interface
		virtual void reshapeIt(int w, int h);

	private:
		::osg::ref_ptr<osgViewer::Viewer> viewer;
		::osg::ref_ptr<::osg::Group> nodeRoot;
		::osg::ref_ptr<::osg::Node> nodeAircraft;
		::osg::ref_ptr<::osg::Node> nodeEngineCW;
		::osg::ref_ptr<::osg::Node> nodeEngineCCW;
		::osg::ref_ptr<::osg::Group> groupAircraft;
		::osg::ref_ptr<::osg::PositionAttitudeTransform> nodeModifiedAircraft;
		::osg::ref_ptr<osgGA::NodeTrackerManipulator> nodeTracker;
		::osg::ref_ptr<osgEarth::MapNode> nodeMap;
		::osg::ref_ptr<::osg::Light> light;
		::osg::ref_ptr<::osg::LightSource> lightSource;
		::osg::ref_ptr<::osgSim::DOFTransform> setupForwardEngine;
		::osg::ref_ptr<::osgSim::DOFTransform> setupRightEngine;
		::osg::ref_ptr<::osgSim::DOFTransform> setupBackEngine;
		::osg::ref_ptr<::osgSim::DOFTransform> setupLeftEngine;
		::osg::observer_ptr<osgViewer::GraphicsWindow> window;
		Basic::safe_ptr<Simulation::UnmannedAirVehicle> uav;
		Basic::safe_ptr<Simulation::Simulation> sim;
		//EngineVisitor* engineVisitor;
	};
} // end of Eaagles namespace

#endif

