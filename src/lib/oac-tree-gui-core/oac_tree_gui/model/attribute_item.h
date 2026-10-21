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

namespace oac_tree_gui
{

/**
 * @brief The AttributeItem class represents a scalar attribute of a domain object.
 *
 * @details The type of the attribute is fixed and given by the default value. The data is one of:
 * - absent: the attribute is not set, the domain applies its default (nullable attributes only);
 * - a scalar of the attribute type: the value;
 * - a string: text, i.e. `$par` or `@var`, if the attribute allows text.
 *
 * A string attribute holds a string. Other data is refused. Empty text of a non-string attribute
 * removes the data, or sets the default value if the attribute is not nullable.
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
   * @param is_nullable The attribute can have no data.
   * @param allow_text The attribute accepts text instead of a value.
   *
   * @details A non-nullable attribute without data gets the default value. A string attribute is
   * never nullable and takes no text: a string is its value.
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
   * @brief Returns true if the attribute holds text instead of a value of its type.
   */
  bool HasText() const;

protected:
  void ActivateStrategies() override;

private:
  //! Accepts the data of the attribute type, text if allowed, or no data for a nullable attribute.
  //! A switch between value and text is one undo step.
  bool OnSetData(mvvm::SessionItem* item, const mvvm::variant_t& value, mvvm::role_t role);
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
