/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_

//! @file
//! Collection of helper functions related to the oac-tree-server.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <memory>
#include <optional>

namespace oac_tree_gui
{

/**
 * @brief Constructs InstructionInfo object from the given instruction.
 */
sup::oac_tree::InstructionInfo CreateInstructionInfo(const sup::oac_tree::Instruction& instruction,
                                                     sup::dto::uint32 index = 0);

/**
 * @brief Constructs VariableInfo object from the given variable.
 */
sup::oac_tree::VariableInfo CreateVariableInfo(const sup::oac_tree::Variable& variable,
                                               sup::dto::uint32 index = 0);

/**
 * @brief Creates domain instruction from automation InstructionInfo presentation.
 *
 * This method ignores all possible children instructions.
 */
std::unique_ptr<sup::oac_tree::Instruction> CreateDomainInstruction(
    const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates domain variable from automation VariableInfo presentation.
 */
std::unique_ptr<sup::oac_tree::Variable> CreateDomainVariable(
    const sup::oac_tree::VariableInfo& info);

/**
 * @brief Returns optional value of the attribute with the given name.
 *
 * The value will be empty, if attribute is not present.
 */
std::optional<std::string> GetAttribute(const sup::oac_tree::InstructionInfo& info,
                                        const std::string& attribute_name);

/**
 * @brief Returns optional value of the attribute with the given name.
 *
 * The value will be empty, if attribute is not present.
 */
std::optional<std::string> GetAttribute(const sup::oac_tree::VariableInfo& info,
                                        const std::string& attribute_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_
