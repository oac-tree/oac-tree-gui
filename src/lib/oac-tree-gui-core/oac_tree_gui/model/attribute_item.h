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

#ifndef OAC_TREE_GUI_MODEL_ATTRIBUTE_ITEM_H_
#define OAC_TREE_GUI_MODEL_ATTRIBUTE_ITEM_H_

#include <mvvm/model/session_item.h>

#include <cstdint>

namespace oac_tree_gui
{

/**
 * @brief The AttributeState enum lists the states of AttributeItem.
 */
enum class AttributeState : std::uint8_t
{
  kNotSet,  //!< no value and no text, the domain applies its default
  kValue,   //!< a value of the attribute type
  kText     //!< text, i.e. `$par` or `@var`
};

/**
 * @brief The AttributeItem class represents a scalar attribute of a domain object.
 *
 * @details The type of the attribute is fixed and given by the default value. The data role holds
 * no data (nullable attributes only) or a value of the attribute type. The text role holds the
 * text, if the attribute allows text.
 *
 * Text sits on top of the value: the value stays and comes back when the text is removed. A string
 * attribute has no text.
 */
class AttributeItem : public mvvm::SessionItem
{
public:
  AttributeItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Sets the attribute type.
   *
   * @param default_value The scalar defining the type and the start value of the editor.
   * @param is_nullable The attribute can have no value.
   * @param allow_text The attribute accepts text instead of a value.
   *
   * @details A non-nullable attribute without value gets the default value. A string attribute is
   * never nullable and takes no text.
   */
  void SetAttributeType(const mvvm::variant_t& default_value, bool is_nullable, bool allow_text);

  /**
   * @brief Returns the default value, which defines the attribute type.
   */
  mvvm::variant_t GetDefaultValue() const;

  /**
   * @brief Returns true if the attribute accepts text instead of a value.
   */
  bool IsTextAllowed() const;

  /**
   * @brief Returns the state of the attribute.
   */
  AttributeState GetState() const;

  /**
   * @brief Returns the text, empty if the attribute has no text.
   */
  std::string GetText() const;

  /**
   * @brief Sets the text. Empty text removes it, the value comes back.
   *
   * @return False if nothing was changed, e.g. the attribute takes no text.
   */
  bool SetText(const std::string& text);

  /**
   * @brief Sets the value and removes the text, in one undo step.
   *
   * @return False if nothing was changed, e.g. the value type doesn't match the attribute type.
   */
  bool SetValue(const mvvm::variant_t& value);

  /**
   * @brief Removes the value and the text, in one undo step.
   *
   * @return False if nothing was changed, e.g. the attribute is not nullable.
   */
  bool Unset();

protected:
  void ActivateStrategies() override;

private:
  //! Accepts no data (nullable only) or a value of the attribute type in the data role, and a
  //! string in the text role if text is allowed. Empty text removes the text role.
  bool OnSetData(mvvm::SessionItem* item, const mvvm::variant_t& value, mvvm::role_t role);

  //! Removes the text and sets the data role to the given variant, in one undo step.
  bool ReplaceTextWithData(const mvvm::variant_t& value);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::AttributeItem>
{
  static constexpr std::string_view type_name() noexcept { return "Attribute"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_ATTRIBUTE_ITEM_H_
