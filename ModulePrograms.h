#ifndef __ModulePrograms_h__
#define __ModulePrograms_h__

#include "Module.h"
#include "GL/glew.h"

class ModulePrograms : public Module
{
public:
	ModulePrograms();
	~ModulePrograms();

	unsigned LoadProgram(const char* vertShaderPath, const char* fragShaderPath);

private:
	char* ReadShaderFile(const char* shaderPath);
	bool CompileShader(unsigned shaderAddress, const char* shaderContent);
	void CompileProgram(unsigned programAddress);
};
#endif // __ModulePrograms_h__