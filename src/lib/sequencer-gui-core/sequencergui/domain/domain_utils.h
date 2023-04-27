/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_

//! Collection of utility functions to access/create various Sequencer related objects.
//! With this file we want to decrease as much as possible the visibility of sequencer headers
//! in the code.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/sequencer_types_fwd.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sequencergui
{

std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name);

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name);

std::vector<std::string> GetDomainInstructionNames();

std::vector<std::string> GetDomainVariableNames();

std::map<std::string, std::string> GetAttributes(const instruction_t* instruction);

std::map<std::string, std::string> GetAttributes(const variable_t* variable);

bool IsRootInstruction(const instruction_t* instruction);

bool IsVariableTypeAvailable(const std::string& domain_type);

bool IsInstructionTypeAvailable(const std::string& domain_type);

bool IsSequencerPluginEpicsAvailable();

void LoadPlugins();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_
