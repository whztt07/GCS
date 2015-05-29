#include "TestMap.h"

namespace Eaagles {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestMap,"TestMap")
EMPTY_SERIALIZER(TestMap)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TestMap::TestMap()
{
    STANDARD_CONSTRUCTOR()
    
		ref_ptr<Group> rootnode = new Group;
	
		ref_ptr<Node> cessna = osgDB::readNodeFile("c:/OpenSceneGraph/data/cessnafire.osg");
		cessna->setDataVariance(::osg::Object::STATIC);
		ref_ptr<MatrixTransform> mtMove = new MatrixTransform;
		mtMove->setDataVariance( ::osg::Object::STATIC );
		mtMove->addChild( cessna.get() );
		rootnode->addChild(mtMove.get());

		ref_ptr<Node> map = osgDB::readNodeFile("c:/Terrain/out.osgb");
		rootnode->addChild(map.get());
		
		viewer = new osgViewer::Viewer;
		window = viewer->setUpViewerAsEmbeddedInWindow(0,0,1000,500);
		viewer->setSceneData(rootnode.get());
		viewer->setCameraManipulator(new osgGA::TrackballManipulator);
		viewer->addEventHandler(new osgViewer::StatsHandler);
		viewer->realize();

}


//------------------------------------------------------------------------------
// copyData()
//------------------------------------------------------------------------------
void TestMap::copyData(const TestMap& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(TestMap)


//------------------------------------------------------------------------------
// updateData() -- update non time-critical stuff here
//------------------------------------------------------------------------------
void TestMap::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
}

void TestMap::draw() {
	// update and render the scene graph
  if (viewer.valid()) 
		viewer->frame();
  // Swap Buffers
  glutSwapBuffers();
  glutPostRedisplay();
	BaseClass::draw();
}

void TestMap::reshapeIt(int w, int h) {
	// update the window dimensions, in case the window has been resized.
	if (window.valid())  {
		window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
		window->getEventQueue()->windowResize(window->getTraits()->x, window->getTraits()->y, w, h );
  }
}

} // end of Eaagles namespace

