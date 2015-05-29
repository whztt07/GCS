#ifndef TEST_HSI
#define TEST_HSI

#include "openeaagles/basicGL/Page.h"

namespace Eaagles {
	class TestHsi : public BasicGL::Page {
	DECLARE_SUBCLASS(TestHsi,BasicGL::Page)
	public:
    TestHsi();
		// Basic::Component interface
    virtual void updateData(const LCreal dt = 0);
	};
} // end of Eaagles namespace

#endif

