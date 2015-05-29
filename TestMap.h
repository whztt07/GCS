//------------------------------------------------------------------------------
// Class: TestHsi
//
// Description: Tests our core HSI graphic
//------------------------------------------------------------------------------
#ifndef TEST_MAP
#define TEST_MAP

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
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
		virtual void draw();

		// Display Interface
		virtual void reshapeIt(int w, int h);

	private:
		ref_ptr<osgViewer::Viewer> viewer;
		observer_ptr<osgViewer::GraphicsWindow> window;
	};
} // end of Eaagles namespace

#endif

