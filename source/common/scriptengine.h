#ifndef X2D_SCRIPTS_H
#define X2D_SCRIPTS_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/base.h>

#include "../../source/scripts/stringstream.h"

// Script func-call argument
struct ScriptValue
{
	ScriptValue();
	~ScriptValue();

	void set(void *value, int typeId);
	void clear();

	bool valid;
	int typeId;
	void *value;
};

// A asIScriptEngine wrapper (mainly used by external plugins)
class XDAPI xdScriptEngine
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	xdScriptEngine(asIScriptEngine *scriptEngine, class xdDebug *debugger);
	~xdScriptEngine();

	// Script module
	asIScriptModule *getModule() const;

	// Script engine
	asIScriptEngine *getASEngine() const;
	
	// Script context
	asIScriptContext *createContext() const;

	// Script classes
	uint classCount() const;
	int classIdByName(const string className) const;
	string classNameById(const uint idx) const;
	bool isClassName(const string className) const;
	bool classDerivesFromName(const string class1, const string class2) const;
	bool classDerivesFromId(const uint id1, const uint id2) const;

	// Script objects
	string objectClassName(void *obj, int typeId) const;

	// Execute string
	void executeString(const string &str) const;

	// Serializing
	void serialize(asIScriptObject *object, string &path);
	void serialize(asIScriptObject *object, StringStream &ss);
	asIScriptObject *deserialize(string &path);
	asIScriptObject *deserialize(StringStream &ss);

private:
	asIScriptModule *m_module;
	class xdDebug *m_debugger;
};

// AngelScript functions
void asMessageCallback(const asSMessageInfo *msg, void *param);
int  asCompileModule(const string &name, class xdFileSystem *fileSystem);

#endif // X2D_SCRIPTS_H