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

#ifndef OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_
#define OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

/**
 * @brief The VariableItem class represents a domain variable of any type.
 *
 * @details The item is set up by CreateVariableItem. Domain attributes are properties of the
 * attribute container, the JSON type and value pair is an AnyValueItem.
 */
class VariableItem : public mvvm::CompoundItem
{
public:
  VariableItem();
  ~VariableItem() override = default;

  VariableItem(const VariableItem&) = delete;
  VariableItem& operator=(const VariableItem&) = delete;
  VariableItem(VariableItem&&) = delete;
  VariableItem& operator=(VariableItem&&) = delete;

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns domain type.
   */
  std::string GetDomainType() const;

  /**
   * @brief Sets the domain type. No attribute initialization.
   */
  void SetDomainType(const std::string& domain_type);

  /**
   * @brief Returns variable name.
   */
  std::string GetName() const;

  /**
   * @brief Sets the name of the variable.
   */
  void SetName(const std::string& value);

  /**
   * @brief Returns the container with domain attributes, or nullptr for a bare item.
   */
  const mvvm::CompoundItem* GetAttributes() const;

  mvvm::CompoundItem* GetAttributes();

  /**
   * @brief Sets the domain attribute to a given value and marks it as exposed.
   *
   * @param name The name of the attribute.
   * @param value The attribute value. Its type must match the attribute type.
   * @throw RuntimeException if the attribute doesn't exist or the value type does not match.
   */
  void SetAttribute(const std::string& name, const mvvm::variant_t& value);

  /**
   * @brief Returns the value of the domain attribute.
   *
   * @tparam T The type of the attribute value.
   * @param name The name of the attribute.
   * @throw RuntimeException if the attribute doesn't exist.
   */
  template <typename T>
  T GetAttribute(const std::string& name) const;

  /**
   * @brief Returns underlying AnyValueItem if exists, or nullptr if no item has been set.
   */
  const sup::gui::AnyValueItem* GetAnyValueItem() const;

  sup::gui::AnyValueItem* GetAnyValueItem();

  /**
   * @brief Checks if variable is available.
   *
   * @details This method just returns the value of the corersponding flag. It is the job of
   * workspace syncronizer to update flag status on domain variable actual status change.
   */
  bool IsAvailable() const;

  /**
   * @brief Sets availability flag to a given value.
   */
  void SetIsAvailable(bool value);

private:
  //! Returns the attribute item with the given name. Throws if it doesn't exist.
  const mvvm::SessionItem& GetAttributeItem(const std::string& name) const;
};

template <typename T>
T VariableItem::GetAttribute(const std::string& name) const
{
  return GetAttributeItem(name).Data<T>();
}

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::VariableItem>
{
  static constexpr std::string_view type_name() noexcept { return "Variable"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_
