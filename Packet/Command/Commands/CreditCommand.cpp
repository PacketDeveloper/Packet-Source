#include "../../../Utils/Utils.h"
#include "CreditCommand.h"

CreditCommand::CreditCommand() : IMCCommand("credits", "Credits", "<how did you get here>") {
	registerAlias("credit");
}

CreditCommand::~CreditCommand() {
}

bool CreditCommand::execute(std::vector<std::string>* args) {
	clientMessageF("==========");
	clientMessageF("%sPacket Client Credits:", GRAY);
	clientMessageF("%sTurakan: TargetHUD & Original LowHop", WHITE);
	clientMessageF("==========");
	return true;
}