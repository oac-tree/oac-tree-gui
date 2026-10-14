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

#ifndef OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_

//! Collection of classes to represent oac-tree instructions.

#include <oac_tree_gui/model/instruction_item.h>

namespace oac_tree_gui
{

/**
 * @brief The UniversalInstructionItem class represents domain Instruction.
 */

class UniversalInstructionItem : public InstructionItem
{
public:
  /**
   * @brief Main c-tor.
   *
   * The item is set up by CreateInstructionItemTree.
   */
  UniversalInstructionItem();
  ~UniversalInstructionItem() override = default;

  UniversalInstructionItem(const UniversalInstructionItem&) = delete;
  UniversalInstructionItem& operator=(const UniversalInstructionItem&) = delete;
  UniversalInstructionItem(UniversalInstructionItem&&) = delete;
  UniversalInstructionItem& operator=(UniversalInstructionItem&&) = delete;

  std::unique_ptr<SessionItem> Clone() const override;
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::UniversalInstructionItem>
{
  static constexpr std::string_view type_name() noexcept { return "UniversalInstruction"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
