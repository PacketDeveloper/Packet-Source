#include "WatermarkCommand.h"

#include "../../Module/ModuleManager.h"

WatermarkCommand::WatermarkCommand() : IMCCommand("clientname", "Edit Clients Name", "<name/reset> <name>") {
	registerAlias("watermark");
	registerAlias("name");
}

WatermarkCommand::~WatermarkCommand() {
}

bool WatermarkCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	auto watermark = moduleMgr->getModule<Watermark>();

	if (args->at(1) == "name" && args->size() > 2) {
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
	} else if (args->at(1) == "reset") {
		watermark->getMessage() = "Packet Client";
		clientMessageF("[%sPacket%s] %sReset!", GRAY, WHITE, GREEN);
		return true;
	}
	return false;
}