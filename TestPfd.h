#ifndef TEST_PFD
#define TEST_PFD

#include <openeaagles/basicGL/Page.h>

namespace Eaagles {
	class TestPfd : public BasicGL::Page 	{
	DECLARE_SUBCLASS(TestPfd,BasicGL::Page)
	public:
		TestPfd();
	  // Basic::Component interface
		virtual void updateData(const LCreal dt = 0);
	};
} // end of Eaagles namespace

#endif

