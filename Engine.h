#ifndef ENGINE
#define ENGINE

#include <osgSim/DOFTransform>

namespace Eaagles {

	class Engine : public ::osgSim::DOFTransform {
	public:
		Engine();
		unsigned int getEngineIndex() const;
		void setEngineIndex(unsigned int EngineIndex);
	private:
		unsigned int mEngineIndex;
	};
}
#endif

