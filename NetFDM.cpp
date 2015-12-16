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

using boost::asio::ip::tcp;
using namespace std;

namespace Eaagles {

	IMPLEMENT_SUBCLASS(NetFDM,"NetFDM")

	//------------------------------------------------------------------------------
	// Slot table
	//------------------------------------------------------------------------------
	EMPTY_SLOTTABLE(NetFDM)
	
	EMPTY_SERIALIZER(NetFDM)

	//------------------------------------------------------------------------------
	// Constructor(s)
	//------------------------------------------------------------------------------
	NetFDM::NetFDM() : acceptor_(new boost::asio::ip::tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 3001))), socket_(new boost::asio::ip::tcp::socket(io_service)) {
		STANDARD_CONSTRUCTOR()
		initData();
	}
	
	std::string& trim_left(std::string& str) {
		while (str.size() && isspace((unsigned char)str[0])) {
			str = str.erase(0, 1);
		}
		return str;
	}

	std::string& trim_right(std::string& str) {
		while (str.size() && isspace((unsigned char)str[str.size() - 1])) {
			str = str.erase(str.size() - 1, 1);
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
		size_t index = 0;
		std::string temp = "";
		trim(str);
		index = str.find(d);
		while (index != std::string::npos) {
			temp = str.substr(0, index);
			trim(temp);
			if (temp.size() > 0) str_array.push_back(temp);
			str = str.erase(0, index + 1);
			index = str.find(d);
		}
		if (str.size() > 0) {
			temp = trim(str);
			if (temp.size() > 0) str_array.push_back(temp);
		}
		return str_array;
	}

	void NetFDM::do_read() {
		//auto self(shared_from_this());
		socket_->async_read_some(boost::asio::buffer(data_, MAX_SIZE), [this](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				Simulation::Player* player = static_cast<Simulation::Player*>(findContainerByType(typeid(Simulation::Player)));
				if (player == 0) {
					std::cerr << "No player" << std::endl;
					return;
				}
				if (length > 0 && length <= MAX_SIZE) {
					std::string data(data_, length);
					std::cerr << "RECV: " << data << std::endl;
					auto string_start = data.find_first_not_of("\r\n", 0);
					if (string_start == std::string::npos)
						return;
					auto string_end = data.find_first_of("\r\n", string_start);
					if (string_end == std::string::npos)
						return;
					std::string line = data.substr(string_start, string_end - string_start);
					if (line.size() == 0)
						return;

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
						return;
					}
					else {
						auto time = stod(trim(tokens[0]));
						auto altitudeASL = stod(trim(tokens[1]));
						auto v_north = stod(trim(tokens[2]));
						auto v_east = stod(trim(tokens[3]));
						auto v_down = stod(trim(tokens[4]));
						auto U = stod(trim(tokens[5]));
						auto V = stod(trim(tokens[6]));
						auto W = stod(trim(tokens[7]));
						auto Roll = stod(trim(tokens[8]));
						auto Pitch = stod(trim(tokens[9]));
						auto Yaw = stod(trim(tokens[10]));
						auto P = stod(trim(tokens[11]));
						auto Q = stod(trim(tokens[12]));
						auto R = stod(trim(tokens[13]));
						auto VelDotX = stod(trim(tokens[14]));
						auto VelDotY = stod(trim(tokens[15]));
						auto VelDotZ = stod(trim(tokens[16]));
						vcas = stod(trim(tokens[17]));
						gLoad = time;

						player->setAltitude(Basic::Distance::FT2M * altitudeASL, true);
						player->setVelocity(static_cast<LCreal>(Basic::Distance::FT2M * v_north), static_cast<LCreal>(Basic::Distance::FT2M * v_east), static_cast<LCreal>(Basic::Distance::FT2M * v_down));
						player->setVelocityBody(static_cast<LCreal>(Basic::Distance::FT2M * U), static_cast<LCreal>(Basic::Distance::FT2M * V), static_cast<LCreal>(Basic::Distance::FT2M * W));
						player->setEulerAngles(static_cast<LCreal>(Roll), static_cast<LCreal>(Pitch), static_cast<LCreal>(Yaw));
						player->setAngularVelocities(static_cast<LCreal>(P), static_cast<LCreal>(Q), static_cast<LCreal>(R));
						player->setAcceleration(static_cast<LCreal>(Basic::Distance::FT2M * VelDotX), static_cast<LCreal>(Basic::Distance::FT2M * VelDotY), static_cast<LCreal>(Basic::Distance::FT2M * VelDotZ));
					}
					do_write();
				}
			}
		});
	}

	void NetFDM::do_write() {
		//auto self(shared_from_this());
		std::string commandToSend;
		commandToSend += std::to_string(aileronCmd) + std::string(",");
		commandToSend += std::to_string(elevatorCmd) + std::string(",");
		commandToSend += std::to_string(rudderCmd) + std::string(",");
		commandToSend += std::to_string(throttleCmd);
		commandToSend += std::string("\r\n");
		std::size_t length = commandToSend.length();
		boost::asio::async_write(*socket_, boost::asio::buffer(data_, length), [this](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				do_read();
			}
		});
	}

	void NetFDM::do_accept() {
		acceptor_->async_accept(*socket_, [this](boost::system::error_code ec) {
			if (!ec) {
				do_read();
			}
		});
	}

	void NetFDM::initData() {
		memset(data_, 0, MAX_SIZE);
		vcas = 0.0;
		gLoad = 0.0;
		socketThread = std::thread(&NetFDM::runThread, this);
	}

	//------------------------------------------------------------------------------
	// copyData() -- copy (delete) member data
	//------------------------------------------------------------------------------
	void NetFDM::copyData(const NetFDM& org, const bool cc) {
		BaseClass::copyData(org);
		if (cc) initData();
	}

	//------------------------------------------------------------------------------
	// deleteData() -- delete instance of NetFDM, if any
	//------------------------------------------------------------------------------
	void NetFDM::deleteData() {
		if (acceptor_ != nullptr) {
			delete acceptor_;
			acceptor_ = nullptr;
		}
		if (socket_ != nullptr) {
			delete socket_;
			socket_ = nullptr;
		}
	}

	LCreal NetFDM::getCalibratedAirspeed() const {
		return vcas;
	}

	LCreal NetFDM::getGload() const {
		return gLoad;
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
			rpm[i] = throttleCmd * 15000.0 * i;
		return max;
	}

	//------------------------------------------------------------------------------
	// dynamics() -- update player's vehicle dynamics
	//------------------------------------------------------------------------------
	void NetFDM::dynamics(const LCreal dt) {
		BaseClass::dynamics(dt);
	}
	
	//------------------------------------------------------------------------------
	// reset() -- 
	//------------------------------------------------------------------------------
	void NetFDM::reset() {
		aileronCmd = 0.0;
		elevatorCmd = 0.0;
		rudderCmd = 0.0;
		throttleCmd = 0.0;
		
		// Get our Player (must have one!)
		Simulation::Player* player = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
		if (player == 0) return;

		BaseClass::reset();
	}	

	void NetFDM::runThread() {
		try {
			do_accept();
			io_service.run();
		}
		catch (std::exception& e) {
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}
} // End Eaagles namespace