//------------------------------------------------------------------------------
// Class: IoHandler
//------------------------------------------------------------------------------
#ifndef IOHANDLER
#define IOHANDLER

#include <openeaagles/basic/IoHandler.h>

namespace Eaagles {
	namespace Simulation { class AirVehicle; }
	//------------------------------------------------------------------------------
	// Class:  IoHandler
	//
	// Description:  Handles the flow of I/O data to and from the
	//               stick and throttle hardware.
	//------------------------------------------------------------------------------
	class IoHandler : public Basic::IoHandler {
		DECLARE_SUBCLASS(IoHandler, Basic::IoHandler)
	public:
		IoHandler();
		// Basic::IoHandler interface functions
		virtual void inputDevices(const LCreal dt);    // Handle input devices
	protected:
		// Basic::IoHandler protected functions
		virtual void clear(); // called from Basic::IoHandler::reset()
	private:
		void initData();
		bool rstSw1;
		bool frzSw1;
		bool wpnReloadSw1;

		bool wpnRelSw1;
		bool trgSw1;
		bool tgtStepSw1;
		bool tgtDesSw1;
		bool rtn2SrchSw1;

		bool autopilotSw1;
		bool incStptSw1;
		bool decStptSw1;
	};
} // end Eaagles namespace

#endif