#include "../../../Utils/Utils.h"
#include "NameCommand.h"

NameCommand::NameCommand() : IMCCommand("Names", "Names", "<how did you get here>") {
	registerAlias("Name");
	registerAlias("idk");
}

NameCommand::~NameCommand() {
}

bool NameCommand::execute(std::vector<std::string>* args) {
	clientMessageF("mmmmm");
	return true;
}