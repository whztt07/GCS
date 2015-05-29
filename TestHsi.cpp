#include "TestHsi.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestHsi,"TestHsi")
	EMPTY_SERIALIZER(TestHsi)

	TestHsi::TestHsi() {
			STANDARD_CONSTRUCTOR()
	}

	void TestHsi::copyData(const TestHsi& org, const bool)	{
		BaseClass::copyData(org);
	}

	EMPTY_DELETEDATA(TestHsi)

	void TestHsi::updateData(const LCreal dt) {
		BaseClass::updateData(dt);
	}
} // end of Eaagles namespace
