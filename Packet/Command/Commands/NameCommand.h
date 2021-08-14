#pragma once
#include <sstream>

#include "ICommand.h"

class NameCommand : public IMCCommand {
public:
	NameCommand();
	~NameCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};