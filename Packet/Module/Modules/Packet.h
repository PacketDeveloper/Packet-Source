
#include "Module.h"
class Packet : public IModule {

public:
	Packet();
	~Packet();
	bool fourx = false;
	bool multiply = false;
	bool noPacket = false;

	// Inherited via IModule
	void onEnable() override;
	bool allowAutoStart() override;
	virtual const char* getModuleName() override;
};
