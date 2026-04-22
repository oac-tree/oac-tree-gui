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

#ifndef OAC_TREE_GUI_MODEL_ITEM_FACTORY_H_
#define OAC_TREE_GUI_MODEL_ITEM_FACTORY_H_

//! @file
//! Collection of helper factory function to build single InstructionItem's and VariableItem's.

#include <memory>
#include <string>

namespace oac_tree_gui
{

class InstructionItem;
class VariableItem;

/**
 * @brief Creates VariableItem from string representing the type of sup::oac_tree::Variable.
 */
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

/**
 * @brief Creates InstructionItem from string representing Type of sup::oac_tree::Instruction.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_ITEM_FACTORY_H_
