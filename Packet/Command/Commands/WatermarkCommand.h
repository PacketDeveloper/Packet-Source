#pragma once
#include <sstream>

#include "ICommand.h"

class WatermarkCommand : public IMCCommand {
public:
	WatermarkCommand();
	~WatermarkCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};