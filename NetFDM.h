//------------------------------------------------------------------------------
// Class: NetFDM
// Description: Handles FDM through IP-network
//------------------------------------------------------------------------------
#ifndef NETFDM
#define NETFDM

#include <boost/asio.hpp>

#include "Base.h"

#include <openeaagles/simulation/dynamics/AerodynamicsModel.h>

#include <thread>
#include <memory>
#include <array>

namespace Eaagles {

	class NetFDM : public Simulation::AerodynamicsModel {
	DECLARE_SUBCLASS(NetFDM, Simulation::AerodynamicsModel)
	public:
		NetFDM();
		
		// AerodynamicsModel interface
		virtual LCreal getCalibratedAirspeed() const;
		virtual LCreal getGload() const;
		virtual int getEngRPM(LCreal* const rpm, const int max) const;
		virtual void setControlStickRollInput(const LCreal roll);
		virtual void setControlStickPitchInput(const LCreal pitch);
		virtual void setRudderPedalInput(const LCreal yaw);
		virtual int setThrottles(const LCreal* const positions, const int num);
							
		// DynamicsModel interface
		virtual void dynamics(const LCreal  dt = 0.0);

		// Component interface
		virtual void reset();
		
	private:
		static const unsigned int MAX_SIZE = 1024;
		char buffer[MAX_SIZE];

		void initData();
		void runThread();
		void do_accept();
		void do_read();
		void do_write();

		std::thread socketThread;
		boost::asio::io_service io_service;
		std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
		std::unique_ptr<boost::asio::ip::tcp::socket> socket;

		LCreal vcas;
		LCreal gLoad;
		double aileronCmd;
		double elevatorCmd;
		double rudderCmd;
		double throttleCmd;
		std::array<LCreal, NUMBER_OF_ENGINES> enginesRPM;
	};
} // End Eaagles namespace

#endif