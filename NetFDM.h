//------------------------------------------------------------------------------
// Class: NetFDM
// Description: Handles FDM through IP-network
//------------------------------------------------------------------------------
#ifndef NETFDM
#define NETFDM

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/basic/String.h"
#include <thread>

namespace Eaagles {

	namespace Basic { class String; class Number; };

	class NetFDM : public Simulation::AerodynamicsModel {
	DECLARE_SUBCLASS(NetFDM, Simulation::AerodynamicsModel)
	public:
		NetFDM();

		static const unsigned int MAX_SIZE = 1024;  // Max buffer size

		// AerodynamicsModel interface
		virtual LCreal getCalibratedAirspeed() const;
		virtual int getEngRPM(LCreal* const rpm, const int max) const;
		virtual void setControlStickRollInput(const LCreal roll);
		virtual void setControlStickPitchInput(const LCreal pitch);
		virtual void setRudderPedalInput(const LCreal yaw);
		virtual int setThrottles(const LCreal* const positions, const int num);
							
		// DynamicsModel interface
		virtual void dynamics(const LCreal  dt = 0.0);

		// Component interface
		virtual void reset();
		
		//NetFDM slots
		const Basic::String* getRootDir() const;  // JSBSim root directory
		virtual bool setRootDir(const Basic::String* const dir);

		const Basic::String* getModel() const;      // JSBSim model
		virtual bool setModel(const Basic::String* const msl);

	protected:
		// Send (transmit) our data buffer; returns true if successful.
		// 'size' just be less than MAX_SIZE.
		virtual bool sendData(const char* const msg, const unsigned int size);

		// Receive a data buffer; returns number of bytes received;
		// 'maxsize' just be less than MAX_SIZE.
		virtual unsigned int recvData(char* const msg, const unsigned int maxsize);

		//Closing connections;
		void closeConnections();
					
	private:
		void initData();
		void socketThread();

		const Basic::String* rootDir;   // root directory for JSBSim models
		const Basic::String* model;     // JSBSim model

		std::thread workerThread;
		bool connected;
		SOCKET listenerSock, clientSock;
		
		LCreal vcas;
		double aileronCmd;
		double elevatorCmd;
		double rudderCmd;
		double throttleCmd;
	};
} // End Eaagles namespace

#endif