/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_

//! @file
//! Collection of utils to test sequencer.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>

#include <string>

namespace oac_tree_gui::test
{

//! Test instruction playing the role of domain instruction unknown to the GUI.
class UnknownDomainInstruction : public ::sup::oac_tree::Instruction
{
public:
  UnknownDomainInstruction() : Instruction(Type) {}

  ::sup::oac_tree::ExecutionStatus ExecuteSingleImpl(::sup::oac_tree::UserInterface&,
                                                     ::sup::oac_tree::Workspace&) override
  {
    return {};
  }
  static inline const std::string Type = "UnknownDomainInstruction";

  static void RegisterUnknownDomainInstruction()
  {
    if (!sup::oac_tree::GlobalInstructionRegistry().IsRegisteredInstructionName(Type))
    {
      sup::oac_tree::RegisterGlobalInstruction<UnknownDomainInstruction>();
    }
  }
};

//! Test instruction playing the role of domain variable unknown to the GUI.
class UnknownDomainVariable : public ::sup::oac_tree::Variable
{
public:
  UnknownDomainVariable() : Variable(Type) {}

  bool GetValueImpl(sup::dto::AnyValue& value) const override { return true; }
  bool SetValueImpl(const sup::dto::AnyValue& value) override { return true; }

  static inline const std::string Type = "UnknownDomainVariable";

  static void RegisterUnknownDomainVariable()
  {
    if (!sup::oac_tree::GlobalVariableRegistry().IsRegisteredVariableName(Type))
    {
      sup::oac_tree::RegisterGlobalVariable<UnknownDomainVariable>();
    }
  }
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_UNIVERSAL_ITEMS_H_
