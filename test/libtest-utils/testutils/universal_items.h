/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_

//! Collection of utils to test sequencer.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/gui/core/dto_types_fwd.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <memory>
#include <string>
#include <vector>

namespace testutils
{

//! Test instruction playing the role of domain instruction unknown to the GUI.
class UnknownDomainInstruction : public ::sup::sequencer::Instruction
{
public:
  UnknownDomainInstruction() : Instruction(Type) {}

  ::sup::sequencer::ExecutionStatus ExecuteSingleImpl(::sup::sequencer::UserInterface&,
                                                      ::sup::sequencer::Workspace&) override
  {
    return {};
  }
  static inline const std::string Type = "UnknownDomainInstruction";

  static void RegisterUnknownDomainInstruction()
  {
    if (!sup::sequencer::GlobalInstructionRegistry().IsRegisteredInstructionName(Type))
    {
      sup::sequencer::RegisterGlobalInstruction<UnknownDomainInstruction>();
    }
  }
};

//! Test instruction playing the role of domain variable unknown to the GUI.
class UnknownDomainVariable : public ::sup::sequencer::Variable
{
public:
  UnknownDomainVariable() : Variable(Type) {}

  bool GetValueImpl(sup::dto::AnyValue& value) const override { return true; }
  bool SetValueImpl(const sup::dto::AnyValue& value) override { return true; }

  static inline const std::string Type = "UnknownDomainVariable";

  static void RegisterUnknownDomainVariable()
  {
    if (!sup::sequencer::GlobalVariableRegistry().IsRegisteredVariableName(Type))
    {
      sup::sequencer::RegisterGlobalVariable<UnknownDomainVariable>();
    }
  }
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_
