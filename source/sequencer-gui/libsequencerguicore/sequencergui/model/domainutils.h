/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_DOMAINUTILS_H
#define SEQUENCERGUI_MODEL_DOMAINUTILS_H

//! @file domainutils.h
//! Collection of utility functions to access/create various Sequencer related objects.
//! With this file we want to decrease as much as possible the visibility of sequencer headers
//! in the code.

#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/sequencer_types.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace sequencergui::DomainUtils
{

std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name);

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name);

std::vector<std::string> GetDomainInstructionNames();

std::vector<std::string> GetDomainVariableNames();

std::map<std::string, std::string> GetAttributes(const instruction_t* instruction);

std::map<std::string, std::string> GetAttributes(const variable_t* variable);

bool IsRootInstruction(const instruction_t* instruction);

bool IsChannelAccessAvailable();

void LoadPlugins();

}  // namespace sequi::DomainUtils

#endif  // SEQUENCERGUI_MODEL_DOMAINUTILS_H
