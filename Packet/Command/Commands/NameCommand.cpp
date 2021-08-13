#include "../../../Utils/Utils.h"
#include "NameCommand.h"

NameCommand::NameCommand() : IMCCommand("name", "Namesss", "<how did you get here>") {
	registerAlias("clientname");
	registerAlias("watermark");
	registerAlias("name");
}

NameCommand::~NameCommand() {
}

bool NameCommand::execute(std::vector<std::string>* args) {
	clientMessageF("mmmmyeswtaremark comamndm");
	return true;
}
