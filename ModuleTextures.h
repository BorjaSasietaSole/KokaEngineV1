#include<list>
#include "Module.h"
#include "Globals.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"
#include "SDL.h"
#include "SDL_image/include/SDL_image.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	unsigned int Load(const char* path);
	void ReloadTexture(const char* newPath, unsigned texture);
	void Unload(unsigned id);
	void SetNewParameter(const char* newPath, unsigned texture, unsigned newTextFilter, unsigned newResizeMethod, unsigned newWrapMethod, unsigned newClampMethod);
	void SwitchMipMaps(const char* newPath, unsigned texture, bool state);

	char* imgFormat;
	unsigned textFilter = 0;
	unsigned resizeMethod = 0;
	unsigned wrapMethod = 0;
	unsigned clampMethod = 0;
	bool generateMipMaps = false;
};