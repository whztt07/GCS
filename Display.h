#ifndef DISPLAY
#define DISPLAY

#include <openeaagles/gui/glut/GlutDisplay.h>

namespace Eaagles {
	namespace Simulation { class Simulation; class Station; }

	class Display : public Glut::GlutDisplay {
	DECLARE_SUBCLASS(Display, Glut::GlutDisplay)
	public:
		Display();
		//BasicGL::Display interface
		virtual void reshapeIt(int w, int h);
		//Basic::Component interface
		virtual void updateData(const LCreal dt = 0.0f);

	protected:
		// Basic::Component protected interface
		virtual bool shutdownNotification();
	};
} // End Eaagles namespace

#endif

