#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderTriangle;
class ModulePrograms;
class ModuleLoader;
class ModuleGui;
class ModuleCamera;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void Tick();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModuleRenderTriangle* triangle = nullptr;
	ModuleLoader* models = nullptr;
	ModulePrograms* programs = nullptr;
	ModuleGui* options = nullptr;
	ModuleCamera* camera = nullptr;

	float lastTickTime = 0.0f;
	float deltaTime = 0.0f;
	float auxTimer = 0.0f;
	int frameCounter = 0;
	int FPS = 0;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif // __Application_H__