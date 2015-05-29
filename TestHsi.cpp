#include "TestHsi.h"

namespace Eaagles {

	IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestHsi,"TestHsi")
	EMPTY_SERIALIZER(TestHsi)

	TestHsi::TestHsi() {
			STANDARD_CONSTRUCTOR()
			heading = 0;
			headingSD.empty();
			headingRate = 20;
			headingROSD.empty();
			dme = 0;
			dmeROSD.empty();
	}

	void TestHsi::copyData(const TestHsi& org, const bool)	{
		BaseClass::copyData(org);
		heading = org.heading;
    headingSD.empty();
    headingRate = org.headingRate;
    headingROSD.empty();
    dme = org.dme;
    dmeROSD.empty();
	}

	EMPTY_DELETEDATA(TestHsi)

	void TestHsi::updateData(const LCreal dt) {
		BaseClass::updateData(dt);
		heading += (headingRate * dt);
    if (heading > 360) {
        heading = 360;
        headingRate = -headingRate;
    }
    if (heading < 0) {
        heading = 0;
        headingRate = -headingRate;
    }
    
    dme += dt;
    if (dme > 100) dme = 0;
                
    send("hsi1", UPDATE_VALUE6, heading, headingSD);
	}
} // end of Eaagles namespace
