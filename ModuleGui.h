#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>

class ModuleGui : public Module 
{
public:
	ModuleGui();
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void HandleInputs(SDL_Event& event); 

	const char* glsl_version;

	ImGuiIO io;

	bool showAboutMenu = false;
	bool showHardwareMenu = false;
	bool requestedExit = false;
	bool showSceneConfig = false;
	bool showTextureConfig = false;
	bool showConsole = false;
	bool showZoomMagnifier = false;
	bool showGuide = false;

	std::vector<float> fps_log;
	std::vector<float> ms_log;

};
#endif __ModuleGui_H__