//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef FACTORY
#define FACTORY

namespace Eaagles {
	namespace Basic { class Object; }
	class Factory {
	public:
		 static Basic::Object* createObj(const char* name);
	protected:
		 Factory();   // prevent object creation
	};
}  // end namespace Eaagles

#endif
