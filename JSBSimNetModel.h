//------------------------------------------------------------------------------
// Class: JSBSimNetModel
// Description: JSBSim net Model
//------------------------------------------------------------------------------
#ifndef __Eaagles_Vehicle_JSBSimNetModel_H__
#define __Eaagles_Vehicle_JSBSimNetModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {

	namespace Basic { class String; class Number; };

	class JSBSimNetModel : public Simulation::AerodynamicsModel {
	DECLARE_SUBCLASS(JSBSimNetModel, Simulation::AerodynamicsModel)
	public:
		JSBSimNetModel();

		static const unsigned int MAX_SIZE = 1024;  // Max buffer size

		// Vehicle interface
		virtual LCreal getCalibratedAirspeed() const;
		virtual int getEngRPM(LCreal* const rpm, const int max) const;
		virtual void setControlStickRollInput(const LCreal roll);
		virtual void setControlStickPitchInput(const LCreal pitch);
		virtual void setRudderPedalInput(const LCreal yaw);
		virtual int setThrottles(const LCreal* const positions, const int num);

		const Basic::String* getRootDir() const;  // JSBSim root directory
		virtual bool setRootDir(const Basic::String* const dir);

		const Basic::String* getModel() const;      // JSBSim model
		virtual bool setModel(const Basic::String* const msl);
					
		// DynamicsModel interface
		virtual void dynamics(const LCreal  dt = 0.0);

		// Component interface
		virtual void reset();
		
	protected:
		bool areNetworksEnabled();
		
		// Initialize the networks
		virtual bool initNetworks();

		// Send (transmit) our data buffer; returns true if successful.
		// 'size' just be less than MAX_SIZE.
		virtual bool sendData(const char* const msg, const unsigned int size);

		// Receive a data buffer; returns number of bytes received;
		// 'maxsize' just be less than MAX_SIZE.
		virtual unsigned int recvData(char* const msg, const unsigned int maxsize);

		//Closing connections;
		void closeConnections();

		bool   networkInitialized;                // Network has been initialized
		bool   networkInitFailed;                 // Network initialization has failed
		bool   networkConnected;                  // Network has been connected 
		bool   noWaitFlag;                        // No wait (unblocked) I/O flag
					
	private:
		void initData();

		const Basic::String* rootDir;   // root directory for JSBSim models
		const Basic::String* model;     // JSBSim model

		SOCKET listenerSock, clientSock;
		fd_set readset;
		timeval timeout;

		LCreal vcas;
		double aileronCmd;
		double elevatorCmd;
		double rudderCmd;
		double throttleCmd;
		double terrainElevation;
	};
} // End Eaagles namespace

#endif