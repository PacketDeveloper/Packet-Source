#include "NameCommand.h"

#include "../../Module/ModuleManager.h"

NameCommand::NameCommand() : IMCCommand("clientname", "Edit Clients Name", "<name>") {
	registerAlias("watermark");
	registerAlias("name");
}

NameCommand::~NameCommand() {
}

bool NameCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	auto watermark = moduleMgr->getModule<Watermark>();

	if (option == "name") {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 1)
				os << " ";
			os << args->at(i);
		}
		std::string text = os.str().substr(1);
		watermark->getMessage() = text;
		clientMessageF("%sName set to %s%s%s!", GREEN, GRAY, text.c_str(), GREEN);
		return true;
	}
}