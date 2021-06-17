/**
 *
 *  version.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include "CommandHandler.h"
#include <drogon/DrObject.h>
using namespace drogon;
namespace drogon_ctl {
class version : public DrObject<version>, public CommandHandler {
public:
	virtual void handleCommand(std::vector<std::string> &parameters) override;
	virtual std::string script() override {
		return "display version of this tool";
	}
	virtual bool isTopCommand() override {
		return true;
	}
	version() {
	}
};
} // namespace drogon_ctl
