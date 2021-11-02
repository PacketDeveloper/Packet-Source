
#include "Module.h"
class Emote : public IModule {

public:
	Emote();
	~Emote();
	bool fourx = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
