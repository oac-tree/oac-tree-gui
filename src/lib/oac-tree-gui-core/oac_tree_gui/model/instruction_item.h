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

class AttributeItem;

/**
 * @brief The InstructionItem class represents a domain instruction of any type.
 *
 * The item is set up by CreateInstructionItemTree.
 */
class InstructionItem : public mvvm::CompoundItem
{
public:
  InstructionItem();
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
   * @brief Returns the container with domain attributes, or nullptr if the item is not set up.
   */
  const mvvm::CompoundItem* GetAttributes() const;

  mvvm::CompoundItem* GetAttributes();

  /**
   * @brief Sets the domain attribute to a given value.
   *
   * @param name The name of the attribute.
   * @param value The attribute value. Its type must match the attribute type.
   * @throw RuntimeException if the attribute doesn't exist or the value type does not match.
   */
  void SetAttribute(const std::string& name, const mvvm::variant_t& value);

  /**
   * @brief Sets the domain attribute to text, i.e. `$par` or `@var`.
   *
   * @throw RuntimeException if the attribute doesn't exist or takes no text.
   */
  void SetAttributeText(const std::string& name, const std::string& text);

  /**
   * @brief Removes the value of the domain attribute, the domain default applies.
   *
   * @throw RuntimeException if the attribute doesn't exist or is not nullable.
   */
  void UnsetAttribute(const std::string& name);

  /**
   * @brief Returns the value of the domain attribute.
   *
   * @tparam T The type of the attribute value.
   * @param name The name of the attribute.
   * @return The attribute value.
   * @details The attribute must have data of the given type.
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

private:
  //! Returns the attribute item with the given name, throws if it doesn't exist.
  AttributeItem& GetAttributeItem(const std::string& name);
};

template <typename T>
T InstructionItem::GetAttribute(const std::string& name) const
{
  return GetAttributes()->Property<T>(name);
}

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::InstructionItem>
{
  static constexpr std::string_view type_name() noexcept { return "Instruction"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
