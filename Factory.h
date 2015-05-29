//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Example_Factory_H__
#define __Eaagles_Example_Factory_H__

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
