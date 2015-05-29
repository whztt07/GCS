#include "TestPfd.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestPfd,"TestPfd")
	EMPTY_SERIALIZER(TestPfd)

	TestPfd::TestPfd() {
		STANDARD_CONSTRUCTOR()
	}
	
	void TestPfd::copyData(const TestPfd& org, const bool) {
		BaseClass::copyData(org);
	}

	EMPTY_DELETEDATA(TestPfd)

	void TestPfd::updateData(const LCreal dt) {
		BaseClass::updateData(dt);
	}
} // end of Eaagles namespace
