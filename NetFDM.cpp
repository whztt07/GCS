#include "NetFDM.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include <string>
#include <vector>
#include <algorithm>

namespace Eaagles {

	IMPLEMENT_SUBCLASS(NetFDM,"NetFDM")

	//------------------------------------------------------------------------------
	// Slot table
	//------------------------------------------------------------------------------
	BEGIN_SLOTTABLE(NetFDM)
			"rootDir",      //  1 root directory for JSBSim models
			"model"         //  2 JSBSim model
	END_SLOTTABLE(NetFDM)
	
	// Map slot table to handles 
	BEGIN_SLOT_MAP(NetFDM)
			ON_SLOT( 1, setRootDir,         Basic::String)
			ON_SLOT( 2, setModel,           Basic::String)
	END_SLOT_MAP()

	EMPTY_SERIALIZER(NetFDM)

	//------------------------------------------------------------------------------
	// Constructor(s)
	//------------------------------------------------------------------------------
	NetFDM::NetFDM() : listenerSock(INVALID_SOCKET), clientSock(INVALID_SOCKET) {
		STANDARD_CONSTRUCTOR()
		initData();
	}
	
	void NetFDM::initData() {
		rootDir = 0;
		model = 0;
		vcas = 0.0;
		connected = false;
	}

	
	//------------------------------------------------------------------------------
	// copyData() -- copy (delete) member data
	//------------------------------------------------------------------------------
	void NetFDM::copyData(const NetFDM& org, const bool cc) {
		BaseClass::copyData(org);
		if (cc) initData();
		
		setRootDir( org.rootDir );
		setModel( org.model );
	}


	std::string& trim_left(std::string& str) {
		while (str.size() && isspace((unsigned char)str[0])) {
			str = str.erase(0,1);
		}
		return str;
	}


	std::string& trim_right(std::string& str) {
		while (str.size() && isspace((unsigned char)str[str.size()-1])) {
			str = str.erase(str.size()-1,1);
		}
		return str;
	}


	std::string& trim(std::string& str) {
		if (str.size() == 0) return str;
		std::string temp_str = trim_right(str);
		return str = trim_left(temp_str);
	}


	bool is_number(const std::string& str) {
		if (str.size())
			return (str.find_first_not_of("+-.0123456789Ee") == std::string::npos);
		else
			return false;
	}


	std::vector <std::string> split(std::string str, char d) {
		std::vector <std::string> str_array;
		size_t index=0;
		std::string temp = "";
		trim(str);
		index = str.find(d);
		while (index != std::string::npos) {
			temp = str.substr(0,index);
			trim(temp);
			if (temp.size() > 0) str_array.push_back(temp);
			str = str.erase(0,index+1);
			index = str.find(d);
		}
		if (str.size() > 0) {
			temp = trim(str);
			if (temp.size() > 0) str_array.push_back(temp);
		}
		return str_array;
	}
	

	//------------------------------------------------------------------------------
	// deleteData() -- delete instance of NetFDM, if any
	//------------------------------------------------------------------------------
	void NetFDM::deleteData() {
		closeConnections();
	}

	
	const Basic::String* NetFDM::getRootDir() const { 
		return rootDir; 
	}


	const Basic::String* NetFDM::getModel() const { 
		return model; 
	}
	

	LCreal NetFDM::getCalibratedAirspeed() const {
		return vcas;
	}


	//------------------------------------------------------------------------------
	// setControlStickRollInput(Roll) --  Control inputs: normalized
	//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
	//------------------------------------------------------------------------------
	void NetFDM::setControlStickRollInput(const LCreal roll) {
		fabs(roll) < 0.00002 ? aileronCmd = 0.0 : aileronCmd = roll;
	}


	//------------------------------------------------------------------------------
	// setControlStickPitchInput(Pitch) --  Control inputs: normalized
	//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
	//------------------------------------------------------------------------------
	void NetFDM::setControlStickPitchInput(const LCreal pitch) {
		fabs(pitch) < 0.00002 ? elevatorCmd = 0.0 : elevatorCmd = pitch;
	}


	//------------------------------------------------------------------------------
	// setRudderPedalInput(Yaw) --  Control inputs: normalized
	//  yaw:  -1.0 -> max left (nose left); 0.0 -> center;  1.0 -> max right (noe right)
	//------------------------------------------------------------------------------
	void NetFDM::setRudderPedalInput(const LCreal yaw) {
		fabs(yaw) < 0.00002 ? rudderCmd = 0.0 : rudderCmd = yaw;
	}

	
	int NetFDM::setThrottles(const LCreal* const positions, const int num) {
		throttleCmd = positions[0];
		return sizeof(positions);
	}


	int NetFDM::getEngRPM(LCreal* const rpm, const int max) const {
		for (auto i = 0; i < max; ++i)
			rpm[i] = throttleCmd * 15000.0;
		return max;
	}


	//------------------------------------------------------------------------------
	// dynamics() -- update player's vehicle dynamics
	//------------------------------------------------------------------------------
	void NetFDM::dynamics(const LCreal dt) {			// Get our Player (must have one!)
		Simulation::Player* p = static_cast<Simulation::Player*>(findContainerByType(typeid(Simulation::Player)));
		if (p == 0) {
			std::cerr << "no player" << std::endl;
			return;
		}
		
		if (!workerThread.joinable()) {
			workerThread = std::thread(&NetFDM::socketThread, this);
		}
		
		BaseClass::dynamics(dt);
	}


	//------------------------------------------------------------------------------
	// reset() -- 
	//------------------------------------------------------------------------------
	void NetFDM::reset() {
		BaseClass::reset();
		
		aileronCmd = 0.0;
		elevatorCmd = 0.0;
		rudderCmd = 0.0;
		throttleCmd = 0.0;
		connected = false;
		
		// Get our Player (must have one!)
		Simulation::Player* p = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
		if (p == 0) return;

		// must have strings set
		if (rootDir == 0 || model == 0) return;
	}	


	//------------------------------------------------------------------------------
	// Slot access functions
	//------------------------------------------------------------------------------


	// Sets root directory for JSBSim models
	bool NetFDM::setRootDir(const Basic::String* const dir) {
		if (rootDir != 0) {
			rootDir->unref();
			rootDir = 0;
		}
		if (dir != 0) {
			rootDir = dir->clone();
		}
		return true;
	}


	// Sets JSBSim model
	bool NetFDM::setModel(const Basic::String* const mdl) {
		if (model != 0) {
			model->unref();
			model = 0;
		}
		if (mdl != 0) {
			model = mdl->clone();
		}
		return true;
	}


	//------------------------------------------------------------------------------
	// getSlotByIndex()
	//------------------------------------------------------------------------------
	Basic::Object* NetFDM::getSlotByIndex(const int si) {
		return BaseClass::getSlotByIndex(si);
	}
	
	
	//------------------------------------------------------------------------------
	// Send (transmit) our data buffer; returns true if successful.
	// 'size' just be less than MAX_SIZE.
	//------------------------------------------------------------------------------
	bool NetFDM::sendData(const char* const msg, const unsigned int size) {
		if (msg != 0 && size > 0 && size < MAX_SIZE) {
			auto n0 = ::send(clientSock, msg, size, 0);
			if (n0 > 0) {
				return true;
			} 
			else if( n0 == SOCKET_ERROR )
				std::cerr << "NetFDM::sendData: " << WSAGetLastError() << std::endl;
		}
		else {
			std::cerr << "NetFDM::sendData(): unable to send data; ";
			if (msg == 0) std::cerr << "No message buffer.";
			if (size == 0 || size >= MAX_SIZE) std::cerr << "invalid message size.";
			std::cerr << std::endl;
		}
		return false;
	}


	//------------------------------------------------------------------------------
	// Receive a data buffer; returns number of bytes received;
	// 'maxsize' just be less than MAX_SIZE.
	//------------------------------------------------------------------------------
	
	unsigned int NetFDM::recvData(char* const msg, const unsigned int maxsize) {
		auto n = 0;
		if (msg != 0 && maxsize > 0 && maxsize <= MAX_SIZE) {
      char buffer[MAX_SIZE];
      auto n0 = recv(clientSock, buffer, MAX_SIZE, 0);
			if (n0 != SOCKET_ERROR && n0 <= MAX_SIZE) {
				std::copy_n(buffer, n0, msg);
				n = n0;
			}
			else if( n0 == SOCKET_ERROR )
				std::cerr << "NetFDM::recvData: " << WSAGetLastError() << std::endl;
		}
		else {
			std::cerr << "NetFDM::recvData(): unable to receive data; ";
			if (msg == 0) std::cerr << "No message buffer.";
			if (maxsize == 0 || maxsize >= MAX_SIZE) std::cerr << "invalid max message size.";
			std::cerr << std::endl;
		}
		return n;
	}

		
	void NetFDM::socketThread() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(1, 1), &wsaData)) {
			std::cerr << "WSAStartup: " << WSAGetLastError() << std::endl;
			return;
		}
		listenerSock = socket(AF_INET, SOCK_STREAM, 0);
		if (listenerSock < 0) {
			std::cerr << "socket: " << WSAGetLastError() << std::endl;
			return;
		}
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(3001);
		addr.sin_addr.s_addr = INADDR_ANY;
		if (bind(listenerSock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			std::cerr << "bind: " << WSAGetLastError() << std::endl;
			return;
		}

		listen(listenerSock, 1);

		clientSock = accept(listenerSock, NULL, NULL);
		if (clientSock < 0) {
			std::cerr << "accept: " << WSAGetLastError() << std::endl;
			return;
		}

		connected = true;
		char buffer[MAX_SIZE];
		std::string data;
		double time, altitudeASL, v_north, v_east, v_down, U, V, W, Roll, Pitch, Yaw, P, Q, R, VelDotX, VelDotY, VelDotZ;

		while(connected) {
			Simulation::Player* player = static_cast<Simulation::Player*>(findContainerByType(typeid(Simulation::Player)));
			if (player == 0) {
				std::cerr << "player: " << std::endl;
				return;
			}
			auto n = recvData(buffer, MAX_SIZE);
			if (n > 0) {
				data.assign(buffer, n);
				std::cerr << "RECV: " << data << std::endl;
				auto string_start = data.find_first_not_of("\r\n", 0);
				if (string_start == std::string::npos)
					continue;
				auto string_end = data.find_first_of("\r\n", string_start);
				if (string_end == std::string::npos)
					continue;
				std::string line = data.substr(string_start, string_end - string_start);
				if (line.size() == 0)
					continue;

				line = trim(line);

				std::vector <std::string> tokens = split(line, ',');
				if ((!is_number(tokens[0])) ||
					(!is_number(tokens[1])) ||
					(!is_number(tokens[2])) ||
					(!is_number(tokens[3])) ||
					(!is_number(tokens[4])) ||
					(!is_number(tokens[5])) ||
					(!is_number(tokens[6])) ||
					(!is_number(tokens[7])) ||
					(!is_number(tokens[8])) ||
					(!is_number(tokens[9])) ||
					(!is_number(tokens[10])) ||
					(!is_number(tokens[11])) ||
					(!is_number(tokens[12])) ||
					(!is_number(tokens[13])) ||
					(!is_number(tokens[14])) ||
					(!is_number(tokens[15])) ||
					(!is_number(tokens[16])) ||
					(!is_number(tokens[17]))) {
					continue;
				}
				else {
					time = stod(trim(tokens[0]));
					altitudeASL = stod(trim(tokens[1]));
					v_north = stod(trim(tokens[2]));
					v_east = stod(trim(tokens[3]));
					v_down = stod(trim(tokens[4]));
					U = stod(trim(tokens[5]));
					V = stod(trim(tokens[6]));
					W = stod(trim(tokens[7]));
					Roll = stod(trim(tokens[8]));
					Pitch = stod(trim(tokens[9]));
					Yaw = stod(trim(tokens[10]));
					P = stod(trim(tokens[11]));
					Q = stod(trim(tokens[12]));
					R = stod(trim(tokens[13]));
					VelDotX = stod(trim(tokens[14]));
					VelDotY = stod(trim(tokens[15]));
					VelDotZ = stod(trim(tokens[16]));
					vcas = stod(trim(tokens[17]));

					player->setAltitude(Basic::Distance::FT2M * altitudeASL, true);
					player->setVelocity(static_cast<LCreal>(Basic::Distance::FT2M * v_north), static_cast<LCreal>(Basic::Distance::FT2M * v_east), static_cast<LCreal>(Basic::Distance::FT2M * v_down));
					player->setVelocityBody(static_cast<LCreal>(Basic::Distance::FT2M * U), static_cast<LCreal>(Basic::Distance::FT2M * V), static_cast<LCreal>(Basic::Distance::FT2M * W));
					player->setEulerAngles(static_cast<LCreal>(Roll), static_cast<LCreal>(Pitch), static_cast<LCreal>(Yaw));
					player->setAngularVelocities(static_cast<LCreal>(P), static_cast<LCreal>(Q), static_cast<LCreal>(R));
					player->setAcceleration(static_cast<LCreal>(Basic::Distance::FT2M * VelDotX), static_cast<LCreal>(Basic::Distance::FT2M * VelDotY), static_cast<LCreal>(Basic::Distance::FT2M * VelDotZ));
				}
				std::string commandToSend;
				commandToSend += std::to_string(aileronCmd) + std::string(",");
				commandToSend += std::to_string(elevatorCmd) + std::string(",");
				commandToSend += std::to_string(rudderCmd) + std::string(",");
				commandToSend += std::to_string(throttleCmd);
				commandToSend += std::string("\r\n");
				if ( sendData(commandToSend.c_str(), commandToSend.length()) )
					std::cerr << "SENT: " << commandToSend << std::endl;
			}
		}
	}


	//------------------------------------------------------------------------------
	// close all network connections
	//------------------------------------------------------------------------------
	void NetFDM::closeConnections() {
		connected = false;
		workerThread.join();
		if ( listenerSock )
			closesocket(listenerSock);
		if ( clientSock )
			closesocket(clientSock);
	}
} // End Eaagles namespace