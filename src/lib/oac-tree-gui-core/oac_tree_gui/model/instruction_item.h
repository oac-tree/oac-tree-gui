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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_

#include <oac_tree_gui/model/instruction_status.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

/**
 * @brief The InstructionItem class is a base for all GUI items representing Sequencer domain
 * instructions.
 */
class InstructionItem : public mvvm::CompoundItem
{
public:
  explicit InstructionItem(const std::string& item_type);
  ~InstructionItem() override = default;

  InstructionItem(const InstructionItem&) = delete;
  InstructionItem& operator=(const InstructionItem&) = delete;
  InstructionItem(InstructionItem&&) = delete;
  InstructionItem& operator=(InstructionItem&&) = delete;

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns domain type.
   */
  std::string GetDomainType() const;

  /**
   * @brief Sets the domain type. No attribute setup.
   */
  void SetDomainType(const std::string& domain_type);

  /**
   * @brief Returns all child instructions.
   */
  std::vector<const InstructionItem*> GetInstructions() const;

  std::vector<InstructionItem*> GetInstructions();

  /**
   * @brief Sets the domain attribute to a given value and marks it as exposed.
   *
   * @param name The name of the attribute.
   * @param value The attribute value. Its type must match the attribute type.
   * @throw mvvm::RuntimeException if the value type does not match.
   */
  void SetAttribute(const std::string& name, const mvvm::variant_t& value);

  /**
   * @brief Returns the value of the domain attribute.
   *
   * The exposed flag is not checked.
   *
   * @tparam T The type of the attribute value.
   * @param name The name of the attribute.
   * @return The attribute value.
   */
  template <typename T>
  T GetAttribute(const std::string& name) const;

  /**
   * @brief Checks if this instruction has root flag set.
   */
  bool IsRoot() const;

  /**
   * @brief Sets root flag to a given value.
   */
  void SetIsRootFlag(bool value);

  /**
   * @brief Returns instruction name.
   *
   * This is where domain description information is stored.
   */
  std::string GetName() const;

  /**
   * @brief Sets the name to a given value.
   */
  void SetName(const std::string& name);

  /**
   * @brief Returns instruction status.
   */
  InstructionStatus GetStatus() const;

  /**
   * @brief Sets status to a given value.
   */
  void SetStatus(InstructionStatus status);

  /**
   * @brief Retuns x-coordinate of instruction on NodeEditor graphics scene.
   */
  double GetX() const;

  /**
   * @brief Set NodeEditor graphics scene x-coordinate to a given value.
   */
  void SetX(double value);

  /**
   * @brief Retuns y-coordinate of instruction on NodeEditor graphics scene.
   */
  double GetY() const;

  /**
   * @brief Set NodeEditor graphics scene y-coordinate to a given value.
   */
  void SetY(double value);

  /**
   * @brief Sets instruction behavior to the given value.
   *
   * The value must be one from kNativeBehavior, kHiddenBehavior, kSucceedBehavior or kFailBehavior.
   */
  void SetBehavior(const std::string& behavior_name);

  /**
   * @brief Returns the vlue of behavior property.
   */
  std::string GetBehavior() const;
};

template <typename T>
T InstructionItem::GetAttribute(const std::string& name) const
{
  return Property<T>(name);
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
