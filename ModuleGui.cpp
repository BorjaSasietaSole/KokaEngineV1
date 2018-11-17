#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleGui.h"
#include "ModuleCamera.h"

#include "GL/glew.h"
#include "SDL.h"

static void ShowMenuBar();
static void ShowUserGuide();
static void ShowAbout();
static void ShowHardware();
static void ShowSceneConfig(std::vector<float> fps, std::vector<float> ms);
static void ShowTextureConfig();
static void ShowConsole();
static void ShowZoomMagnifier();
static void PrintTextureParams(const char* currentTexture);
static void PrintMipMapOption(const char* currentTexture);

ModuleGui::ModuleGui() {
	fps_log.resize(100);
	ms_log.resize(100);
}

ModuleGui::~ModuleGui() {
}

bool ModuleGui::Init() {
	glsl_version = "#version 130";

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleGui::PreUpdate() {
	fps_log.erase(fps_log.begin());
	fps_log.push_back(App->FPS);
	ms_log.erase(ms_log.begin());
	ms_log.push_back(App->deltaTime * 1000);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update() {

	update_status ret = UPDATE_CONTINUE;
	ShowMenuBar();

	if (showGuide) {
		ShowUserGuide();
	}

	if (showAboutMenu) {
		ShowAbout();
	}

	if (showHardwareMenu) {
		ShowHardware();
	}

	if (showSceneConfig) {
		ShowSceneConfig(fps_log, ms_log);
	}

	if (showTextureConfig) {
		ShowTextureConfig();
	}

	if (showConsole) {
		ShowConsole();
	}

	if (showZoomMagnifier) {
		ShowZoomMagnifier();
	}

	if (requestedExit)
		ret = UPDATE_STOP;

	return ret;
}

update_status ModuleGui::PostUpdate() {
	
	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp() {
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGui::HandleInputs(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

// General menu options
static void ShowMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) { App->options->requestedExit = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Configuration")) { App->options->showSceneConfig = true; }
			if (ImGui::MenuItem("Textures")) { App->options->showTextureConfig = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("Console")) { App->options->showConsole = true; }
			if (ImGui::MenuItem("Hardware")) { App->options->showHardwareMenu = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About")) { App->options->showAboutMenu = true; }
			if (ImGui::MenuItem("Guide Enguine")) { App->options->showGuide = true; }
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}

// Hardware
static void ShowHardware() {
	ImGui::Begin("Hardware specs", &App->options->showHardwareMenu, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("CPU Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "%d", SDL_GetCPUCount());
	ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "%d", SDL_GetSystemRAM());
	ImGui::End();
}

// Scene config
static void ShowSceneConfig(std::vector<float> fps, std::vector<float> ms) {
	ImGui::Begin("Camera", &App->options->showSceneConfig, ImGuiWindowFlags_AlwaysAutoResize);
	bool fovXEdited = false, fovYEdited = false;
	if (ImGui::CollapsingHeader("Performance")) {
		char title[25];
		sprintf_s(title, 25, "Framerate %0.1f", fps[fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms[ms.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Camera properties")) {
		float camPos[3] = { App->camera->cameraPos.x, App->camera->cameraPos.y, App->camera->cameraPos.z };
		ImGui::InputFloat3("Cam position", camPos, "%.3f");
		ImGui::Separator();
		float front[3] = { App->camera->front.x, App->camera->front.y, App->camera->front.z };
		ImGui::InputFloat3("Front", front, "%.3f");
		float side[3] = { App->camera->side.x, App->camera->side.y, App->camera->side.z };
		ImGui::InputFloat3("Side", side, "%.3f");
		float up[3] = { App->camera->up.x, App->camera->up.y, App->camera->up.z };
		ImGui::InputFloat3("Up", up, "%.3f");
		ImGui::Separator();
		ImGui::InputFloat("Pitch", &App->camera->pitch, 0, 0, 0);
		ImGui::InputFloat("Yaw", &App->camera->yaw, 0, 0, 0);
	}
	if (ImGui::CollapsingHeader("Camera config")) {
		ImGui::SliderFloat("Mov Speed", &App->camera->cameraSpeed, 0.0f, 100.0f);
		ImGui::SliderFloat("Rot Speed", &App->camera->rotationSpeed, 0.0f, 100.0f);
		ImGui::SliderFloat("Mouse Sens", &App->camera->mouseSensitivity, 0.0f, 1.0f);
		ImGui::Separator();
		fovXEdited = ImGui::SliderFloat("Horz. Fov", &App->camera->fovX, 1.0f, 45.0f, "%.00f", 1.0f);
		if (ImGui::IsItemEdited()) {
			App->camera->SetHorizontalFOV(App->camera->fovX);
		}
		
		fovYEdited = ImGui::SliderFloat("Vertical. Fov", &App->camera->fovY, 1.0f, 45.0f, "%.00f", 1.0f);
		if (ImGui::IsItemEdited()) {
			App->camera->SetVerticalFOV(App->camera->fovY);
		}
		ImGui::InputFloat("AspectRatio", &App->camera->screenRatio, 0, 0, "%.3f");
		ImGui::SliderFloat("Near Plane", &App->camera->frustum.nearPlaneDistance, 0.1f, App->camera->frustum.farPlaneDistance);
		ImGui::SliderFloat("Far Plane", &App->camera->frustum.farPlaneDistance, 0.1f, 500.0f);
		ImGui::Separator();
		ImGui::SliderFloat3("Background color", App->renderer->bgColor, 0.0f, 1.0f);
	}
	ImGui::End();
}

//Texture config
static void ShowTextureConfig() {
	ImGui::Begin("Textures", &App->options->showTextureConfig, ImGuiWindowFlags_AlwaysAutoResize);
	const char* items[] = { "./textures/Lenna.png", "./textures/Lennin.dds", "./textures/Lolnope.jpg", "./textures/Lolyes.gif" };
	static const char* current_item = items[0];
	if (ImGui::BeginCombo("Available textures", current_item, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				// App->textures->ReloadTexture(items[n], App->renderer->texture0);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Texture information")) {
		ImGui::InputText("Format", App->textures->imgFormat, sizeof(App->textures->imgFormat));
	}
	if (ImGui::CollapsingHeader("Texture config")) {
		PrintTextureParams(current_item);
		ImGui::Separator();
		PrintMipMapOption(current_item);
	}

	ImGui::End();
}

// Texture functions
static void PrintTextureParams(const char* currentTexture) {
	// Wrap methods
	const char* wrapMethods[] = { "GL_TEXTURE_WRAP_R", "GL_TEXTURE_WRAP_S", "GL_TEXTURE_WRAP_T" };
	const int wrapMethodsValues[] = { GL_TEXTURE_WRAP_R, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
	static const char* currentWrap = wrapMethods[0];
	if (ImGui::BeginCombo("Wrap methods", currentWrap, ImGuiComboFlags_NoArrowButton))
	{
		for (int wr = 0; wr < IM_ARRAYSIZE(wrapMethods); wr++)
		{
			bool wrapSelected = (currentWrap == wrapMethods[wr]);
			if (ImGui::Selectable(wrapMethods[wr], wrapSelected)) {
				currentWrap = wrapMethods[wr];
				// App->textures->SetNewParameter(currentTexture, App->exercise->texture0, App->textures->textFilter, App->textures->resizeMethod, wrapMethodsValues[wr], App->textures->clampMethod);
			}
			if (wrapSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	// Resize methods
	const char* resizeMethods[] = { "Linear", "Nearest" };
	const int resizeMethodsValues[] = { GL_LINEAR, GL_NEAREST };
	static const char* currentResize = resizeMethods[0];
	if (ImGui::BeginCombo("Resize methods", currentResize, ImGuiComboFlags_NoArrowButton))
	{
		for (int rs = 0; rs < IM_ARRAYSIZE(resizeMethods); rs++)
		{
			bool resizeSelected = (currentResize == resizeMethods[rs]);
			if (ImGui::Selectable(resizeMethods[rs], resizeSelected)) {
				currentResize = resizeMethods[rs];
				// App->textures->SetNewParameter(currentTexture, App->exercise->texture0, App->textures->textFilter, resizeMethodsValues[rs], App->textures->wrapMethod, App->textures->clampMethod);
			}
			if (resizeSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	// Clamp methods
	const char* clampMethods[] = { "GL_CLAMP", "GL_CLAMP_TO_BORDER", "GL_REPEAT", "GL_MIRRORED_REPEAT" };
	const int clampMethodsValues[] = { GL_CLAMP, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT };
	static const char* currentClamp = clampMethods[0];
	if (ImGui::BeginCombo("Clamp methods", currentClamp, ImGuiComboFlags_NoArrowButton))
	{
		for (int cl = 0; cl < IM_ARRAYSIZE(clampMethods); cl++)
		{
			bool clampSelected = (currentClamp == clampMethods[cl]);
			if (ImGui::Selectable(clampMethods[cl], clampSelected)) {
				currentClamp = clampMethods[cl];
				// App->textures->SetNewParameter(currentTexture, App->exercise->texture0, App->textures->textFilter, App->textures->resizeMethod, App->textures->wrapMethod, clampMethodsValues[cl]);
			}
			if (clampSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	// Texture filter methods
	const char* filterMethods[] = { "GL_TEXTURE_MIN_FILTER", "GL_TEXTURE_MAG_FILTER" };
	const int filterMethodsValues[] = { GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER };
	static const char* currentFilter = filterMethods[0];
	if (ImGui::BeginCombo("Filter methods", currentFilter, ImGuiComboFlags_NoArrowButton))
	{
		for (int fl = 0; fl < IM_ARRAYSIZE(filterMethods); fl++)
		{
			bool filterSelected = (currentFilter == filterMethods[fl]);
			if (ImGui::Selectable(filterMethods[fl], filterSelected)) {
				currentFilter = filterMethods[fl];
				// App->textures->SetNewParameter(currentTexture, App->exercise->texture0, filterMethodsValues[fl], App->textures->resizeMethod, App->textures->wrapMethod, App->textures->clampMethod);
			}
			if (filterSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

static void PrintMipMapOption(const char* currentTexture) {
	const char* mipMapState[] = { "Disabled", "Enabled" };
	bool valueMipMapValue[] = { false, true };
	static const char* currentMMState = mipMapState[0];
	if (ImGui::BeginCombo("MipMap", currentMMState, ImGuiComboFlags_NoArrowButton))
	{
		for (int mm = 0; mm < IM_ARRAYSIZE(mipMapState); mm++)
		{
			bool is_selected = (currentMMState == mipMapState[mm]);
			if (ImGui::Selectable(mipMapState[mm], is_selected)) {
				currentMMState = mipMapState[mm];
				// App->textures->SwitchMipMaps(currentTexture, App->exercise->texture0, valueMipMapValue[mm]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

static void ShowZoomMagnifier() {
	ImGuiWindowFlags zommingFlags = ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Zooming", &App->options->showHardwareMenu, zommingFlags);
	ImGui::InputFloat("Zoom value", &App->camera->zoomValue, 0, 0, 2);
	ImGui::End();
}

static void ShowConsole() {
	PrintConsole("Console", &App->options->showConsole);
}

// About
static void ShowAbout() {

	const char* MITLicense = "@Copyright 2018 - Koka Engine \n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions.";

	ImGui::Begin("About", &App->options->showAboutMenu);

	ImGui::Text(TITLE);
	ImGui::Text("C/C++ engine for game development");
	ImGui::Separator();
	ImGui::Text("Libraries:");
	if (ImGui::MenuItem("SDL v2.0.8")) { ShellExecute(0, 0, "https://www.libsdl.org/index.php", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("Glew v2.1.0")) { ShellExecute(0, 0, "http://glew.sourceforge.net/", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("ImGui v1.66")) { ShellExecute(0, 0, "https://github.com/ocornut/imgui/tree/docking", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("Devil v1.8.0")) { ShellExecute(0, 0, "http://openil.sourceforge.net/", 0, 0, SW_SHOW); }
	ImGui::Separator();
	ImGui::Text("Author:");
	if (ImGui::MenuItem("Borja Sasieta Sole")) { ShellExecute(0, 0, "https://github.com/BorjaSasietaSole/", 0, 0, SW_SHOW); }
	ImGui::Separator();
	ImGui::TextWrapped(MITLicense);
	ImGui::End();
}

static void ShowUserGuide()
{
	ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::BulletText("Click and drag on lower right corner to resize window\n(double-click to auto fit window to its contents).");
	ImGui::BulletText("Click and drag on any empty space to move window.");
	ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
	ImGui::BulletText("CTRL+Click on a slider or drag box to input value as text.");
	if (ImGui::GetIO().FontAllowUserScaling)
		ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
	ImGui::BulletText("Mouse Wheel to scroll.");
	ImGui::BulletText("While editing text:\n");
	ImGui::Indent();
	ImGui::BulletText("Hold SHIFT or use mouse to select text.");
	ImGui::BulletText("CTRL+Left/Right to word jump.");
	ImGui::BulletText("CTRL+A or double-click to select all.");
	ImGui::BulletText("CTRL+X,CTRL+C,CTRL+V to use clipboard.");
	ImGui::BulletText("CTRL+Z,CTRL+Y to undo/redo.");
	ImGui::BulletText("ESCAPE to revert.");
	ImGui::BulletText("You can apply arithmetic operators +,*,/ on numerical values.\nUse +- to subtract.");
	ImGui::Unindent();
}