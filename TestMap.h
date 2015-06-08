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
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osg/Camera>
#include <osg/MatrixTransform>

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Timers.h"
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basicGL/Page.h"

#include "openeaagles/gui/glut/GlutDisplay.h"
#include <GL/glut.h>

#include "openeaagles/gui/glut/Factory.h"
#include "openeaagles/basic/Factory.h"
#include "openeaagles/basicGL/Factory.h"
#include "openeaagles/instruments/Factory.h"

using namespace osg;

namespace Eaagles {
	class TestMap : public BasicGL::Page {
		DECLARE_SUBCLASS(TestMap, BasicGL::Page)
	public:
    TestMap();

    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
		
		// Graphic Interface
		virtual void drawFunc(void);

	private:
		ref_ptr<osgViewer::Viewer> viewer;
		ref_ptr<Group> rootnode;
		ref_ptr<Node> cessna;
		ref_ptr<MatrixTransform> mtMove;
		ref_ptr<MatrixTransform> terrain;
		ref_ptr<osgGA::NodeTrackerManipulator> nodeTracker;
	};
} // end of Eaagles namespace

#endif

