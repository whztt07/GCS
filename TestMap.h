//------------------------------------------------------------------------------
// Class: TestHsi
//
// Description: Tests our core HSI graphic
//------------------------------------------------------------------------------
#ifndef TEST_MAP
#define TEST_MAP

#include <osgViewer/Viewer>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/LightSource>

#include "SimStation.h"

#include <openeaagles/basic/Pair.h>
#include <openeaagles/basic/Timers.h>
#include <openeaagles/basic/Parser.h>
#include <openeaagles/basicGL/Graphic.h>
#include <openeaagles/basicGL/Page.h>
#include <openeaagles/simulation/AirVehicle.h>
#include <openeaagles/simulation/Simulation.h>

#include <openeaagles/gui/glut/GlutDisplay.h>
#include <GL/glut.h>

#include <openeaagles/gui/glut/Factory.h>
#include <openeaagles/basic/Factory.h>
#include <openeaagles/basicGL/Factory.h>
#include <openeaagles/instruments/Factory.h>

namespace Eaagles {
	class TestMap : public BasicGL::Page {
		DECLARE_SUBCLASS(TestMap, BasicGL::Page)
	public:
    TestMap();

    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
		
		virtual void draw();

		virtual bool onEntry();

	private:
		::osg::ref_ptr<osgViewer::Viewer> viewer;
		::osg::ref_ptr<::osg::Group> rootnode;
		::osg::ref_ptr<::osg::Node> cessna;
		::osg::ref_ptr<::osg::Node> map;
		::osg::ref_ptr<::osg::PositionAttitudeTransform> movedCessna;
		::osg::ref_ptr<osgGA::NodeTrackerManipulator> nodeTracker;
		Simulation::AirVehicle* av;
		Simulation::Simulation* sim;
	};
} // end of Eaagles namespace

#endif

