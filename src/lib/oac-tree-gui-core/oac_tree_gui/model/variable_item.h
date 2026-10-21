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

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

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
 * @details The item is set up by SetDomainType or InitFromDomain. Domain attributes are
 * properties, the JSON type and value pair is an AnyValueItem.
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
   * @brief Sets the domain type, the display name and the attributes of the domain type.
   *
   * @throw LogicErrorException if the item is already set up.
   */
  void SetDomainType(const std::string& domain_type);

  /**
   * @brief Inits given item from the domain variable.
   *
   * @param variable The domain variable.
   * @param registry Optional type registry containing already existing domain types.
   */
  void InitFromDomain(const variable_t* variable, const anytype_registry_t* registry = nullptr);

  /**
   * @brief Creates domain variable corresponding to a given item.
   */
  std::unique_ptr<variable_t> CreateDomainVariable() const;

  /**
   * @brief Returns variable name.
   */
  std::string GetName() const;

  /**
   * @brief Sets the name of the variable.
   */
  void SetName(const std::string& value);

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

protected:
  /**
   * @brief Constructor for derived items with their own type name.
   */
  explicit VariableItem(const std::string& item_type);

  /**
   * @brief Provides tag registration for AnyValueItem insertion.
   */
  void RegisterAnyValueItemTag();

  /**
   * @brief Sets the value of domain type role.
   *
   * No attribute initialization.
   */
  void SetDomainTypeRole(const std::string& domain_type);

private:
  /**
   * @brief Implementation to provide to init this VariableItem from the domain variable.
   */
  virtual void InitFromDomainImpl(const variable_t* variable, const anytype_registry_t* registry);

  /**
   * @brief Implementation to provide to setup domain variable from this VariableItem.
   */
  virtual void SetupDomainImpl(variable_t* variable) const;

  //! Sets the domain type, the display name and a property per domain attribute definition.
  void SetupFromDomain(const variable_t* variable);

  //! Returns property items representing domain attributes.
  std::vector<sup::gui::AnyValueItem*> GetAttributeItems() const;
};

template <typename T>
T VariableItem::GetAttribute(const std::string& name) const
{
  return Property<T>(name);
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
