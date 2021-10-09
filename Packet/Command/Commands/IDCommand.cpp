#include "IDCommand.h"

IDCommand::IDCommand() : IMCCommand("id", "Get item/block/entity IDS", "<pointing/holding/entity>") {
}

IDCommand::~IDCommand() {
}

bool IDCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	if (args->at(1) == "pointing") {
		PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
		int id = (int)block->toLegacy()->blockId;
		char* name = block->toLegacy()->name.getText();
		clientMessageF("%sPacket: %sBlock Name: %s", GRAY, WHITE, name);
		clientMessageF("%sPacket: %sBlock ID: %d", GRAY, WHITE, id);
		return true;
	} else if (args->at(1) == "holding") {
		clientMessageF("%sPacket: %sBlock ID: none", GRAY, WHITE);
		return true;
	} else if (args->at(1) == "entity") {
		//int id = ent->getEntityTypeId();
		//char* name = ent->getNameTag()->getText();
		//clientMessageF("%sPacket: %Entity Name: %s", GRAY, WHITE, name);
		//clientMessageF("%sPacket: %Entity ID: %d", GRAY, WHITE, id);
		//return true;
	}
	return false;
}