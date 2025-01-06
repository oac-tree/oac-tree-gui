/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_automation_helper.h"

#include "domain_helper.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_info.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_info.h>

namespace sequencergui
{

std::unique_ptr<sup::sequencer::Instruction> CreateDomainInstruction(
    const sup::sequencer::InstructionInfo &info)
{
  auto result = CreateDomainInstruction(info.GetType());
  result->AddAttributes(info.GetAttributes());
  return result;
}

std::unique_ptr<sup::sequencer::Variable> CreateDomainVariable(
    const sup::sequencer::VariableInfo &info)
{
  auto result = CreateDomainVariable(info.GetType());
  result->AddAttributes(info.GetAttributes());
  return result;
}

}  // namespace sequencergui
