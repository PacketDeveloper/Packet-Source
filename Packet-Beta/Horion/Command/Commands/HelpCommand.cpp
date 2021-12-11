#include "HelpCommand.h"

#include "../CommandMgr.h"

HelpCommand::HelpCommand() : IMCCommand("commands", "Lists all commands", "") {
	registerAlias("command");
}

HelpCommand::~HelpCommand() {
}

bool HelpCommand::execute(std::vector<std::string>* args) {
	clientMessageF("==========");
	std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
	for (auto it = commandList->begin(); it != commandList->end(); ++it) {
		IMCCommand* plump = *it;
		clientMessageF("%s%s - %s%s", plump->getCommand(), GRAY, ITALIC, plump->getDescription());
	}
	return true;
}