#ifndef CORE_BASE_H
#define CORE_BASE_H

// Standard libraries
#include <vector>
#include <list>
#include <string>
#include <utility>

using namespace std;

#include <unordered_map>
#include <string>

class asIScriptEngine;

//-----------------------------------------------------------------------
// Script object base class
//-----------------------------------------------------------------------

class Base
{
public:
    virtual ~Base() { }

    typedef int (*RegisterScriptClassFunc)(asIScriptEngine*);
    typedef int (*DeclareScriptClassFunc)(asIScriptEngine*);

	static int Register(asIScriptEngine *scriptEngine)
    {
		// Error value
		int i = 0;
		
		// Register all script classes
		for(list<DeclareScriptClassFunc>::iterator itr = classesToDeclare().begin(); itr != classesToDeclare().end(); ++itr)
		{
			if((i = (*itr)(scriptEngine)) < 0)
			{
				// An error occured while declaring a class
				return i;
			}
		}

		// Register all script classes
		for(list<RegisterScriptClassFunc>::iterator itr = classesToRegister().begin(); itr != classesToRegister().end(); ++itr)
		{
			if((i = (*itr)(scriptEngine)) < 0)
			{
				// An error occured while registering a class
				return i;
			}
		}
		return i;
    }

    struct Registerer
    {
        explicit Registerer(DeclareScriptClassFunc declFunc, RegisterScriptClassFunc regFunc)
        {
			classesToDeclare().push_back(declFunc);
			classesToRegister().push_back(regFunc);
        }

	private:
		// Don't allow copying of registrars
		Registerer(const Registerer &) {}
    };

private:
    static list<DeclareScriptClassFunc> &classesToDeclare()
	{
		static list<DeclareScriptClassFunc> lst;
		return lst;
	}

    static list<RegisterScriptClassFunc> &classesToRegister()
	{
		static list<RegisterScriptClassFunc> lst;
		return lst;
	}
};

//-----------------------------------------------------------------------
// A simple refcounter
//-----------------------------------------------------------------------

// We use this reference counter to force the user to 
// to initialize it.
class RefCounter
{
public:
	RefCounter(void *owner) :
		refCount(1),
		owner(owner)
	{
	}

	void add()
	{
		refCount++;
	}

	void release()
	{
		if(--refCount == 0)
			delete owner;
	}

private:
	RefCounter(RefCounter &) { };
	int refCount;
	void *owner;
};

//-----------------------------------------------------------------------
// AngelScript macros
//-----------------------------------------------------------------------

#ifdef USING_AS
#include <angelscript.h>
#endif

#define AS_ASSERT \
	if(r < 0) return r;

#define AS_REGISTER_REF(clazz)															\
	Base::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);				\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = scriptEngine->RegisterObjectType(#clazz, 0, asOBJ_REF); AS_ASSERT		\
		r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_ADDREF, "void f()",  \
						asMETHOD(clazz, addRef), asCALL_THISCALL); AS_ASSERT			\
		r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_RELEASE, "void f()", \
						asMETHOD(clazz, release), asCALL_THISCALL); AS_ASSERT			\
		return r;																		\
	}																					\


#define AS_REGISTER_VALUE(clazz)														\
	Base::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);				\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = scriptEngine->RegisterObjectType(#clazz, sizeof(clazz),					\
						asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK); AS_ASSERT		\
		return r;																		\
	}																					\

#define AS_REF_CLASS(clazz)																\
	private:																			\
	RefCounter refCounter;																\
	static Base::Registerer s_basereg;													\
	void addRef() { refCounter.add(); }													\
	void release() { refCounter.release(); }											\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	public:												

#define AS_VALUE_CLASS(clazz)															\
	private:																			\
	static Base::Registerer s_basereg;													\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	public:			

// TODO: I wish there was some way to merge these, but evidently not
#define AS_FACTORY_ARG0(clazz) static clazz *Factory() { return new clazz(); }
#define AS_FACTORY_ARG1(clazz, param0) static clazz *Factory(param0 arg0) { return new clazz(arg0); }	
#define AS_FACTORY_ARG2(clazz, param0, param1) static clazz *Factory(param0 arg0, param1 arg1) { return new clazz(arg0, arg1); }
#define AS_FACTORY_ARG3(clazz, param0, param1, param2) static clazz *Factory(param0 arg0, param1 arg1, param2 arg2) { return new clazz(arg0, arg1, arg2); }
#define AS_FACTORY_ARG4(clazz, param0, param1, param2, param3) static clazz *Factory(param0 arg0, param1 arg1, param2 arg2, param3 arg3) { return new clazz(arg0, arg1, arg2, arg3); }
#define AS_FACTORY_ARG5(clazz, param0, param1, param2, param3, param4) static clazz *Factory(param0 arg0, param1 arg1, param2 arg2, param3 arg3, param4 arg4) { return new clazz(arg0, arg1, arg2, arg3, arg4); }

#define AS_CONSTRUCTOR_ARG0(clazz) static void Construct(clazz *self) { new (self) clazz(); }
#define AS_CONSTRUCTOR_ARG1(clazz, param0) static void Construct(param0 arg0, clazz *self) { new (self) clazz(arg0); }	
#define AS_CONSTRUCTOR_ARG2(clazz, param0, param1) static void Construct(param0 arg0, param1 arg1, clazz *self) { new (self) clazz(arg0, arg1); }
#define AS_CONSTRUCTOR_ARG3(clazz, param0, param1, param2) static void Construct(param0 arg0, param1 arg1, param2 arg2, clazz *self) { new (self) clazz(arg0, arg1, arg2); }
#define AS_CONSTRUCTOR_ARG4(clazz, param0, param1, param2, param3) static void Construct(param0 arg0, param1 arg1, param2 arg2, param3 arg3, clazz *self) { new (self) clazz(arg0, arg1, arg2, arg3); }
#define AS_CONSTRUCTOR_ARG5(clazz, param0, param1, param2, param3, param4) static void Construct(param0 arg0, param1 arg1, param2 arg2, param3 arg3, param4 arg4, clazz *self) { new (self) clazz(arg0, arg1, arg2, arg3, arg4); }

#define AS_COPY_CONSTRUCT(clazz1, clazz2) static void clazz1::CopyConstruct(const clazz2 &other, clazz1 *self) { new (self) clazz1(other); }

#define AS_REGISTER_FACTORY(clazz, decl, params) r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_FACTORY, #clazz " @f(" decl ")", asFUNCTIONPR(clazz::Factory, params, clazz*), asCALL_CDECL); AS_ASSERT
#define AS_REGISTER_METHOD(clazz, decl, func, params, ret) r = scriptEngine->RegisterObjectMethod(#clazz, decl, asMETHODPR(clazz, func, params, ret), asCALL_THISCALL); AS_ASSERT
#define AS_REGISTER_MEMBER(clazz, decl, membr) r = scriptEngine->RegisterObjectProperty(#clazz, decl, offsetof(clazz, membr)); AS_ASSERT
#define AS_REGISTER_CONSTRUCTOR(clazz, decl, params) r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_CONSTRUCT, "void f(" decl ")", asFUNCTIONPR(clazz::Construct, params, void), asCALL_CDECL_OBJLAST); AS_ASSERT

#endif // CORE_BASE_H