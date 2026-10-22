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

#ifndef OAC_TREE_GUI_MODEL_ATTRIBUTE_CONTAINER_HELPER_H_
#define OAC_TREE_GUI_MODEL_ATTRIBUTE_CONTAINER_HELPER_H_

//! @file
//! Functions to access domain attributes in the attribute container of an instruction or variable
//! item.

#include <mvvm/core/variant.h>

#include <string>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

class AttributeItem;

/**
 * @brief Returns the attribute item with the given name.
 *
 * @param item The instruction or variable item with the attribute container.
 * @param name The name of the attribute.
 * @return The attribute item.
 * @throw RuntimeException if the item has no such attribute.
 */
const AttributeItem& GetAttributeItem(const mvvm::SessionItem& item, const std::string& name);

AttributeItem& GetAttributeItem(mvvm::SessionItem& item, const std::string& name);

/**
 * @brief Sets the attribute to a given value and removes its text.
 *
 * @param item The instruction or variable item with the attribute container.
 * @param name The name of the attribute.
 * @param value The attribute value. Its type must match the attribute type.
 * @throw RuntimeException if the attribute doesn't exist or the value type does not match.
 */
void SetAttributeValue(mvvm::SessionItem& item, const std::string& name,
                       const mvvm::variant_t& value);

/**
 * @brief Returns the value of the attribute.
 *
 * @param item The instruction or variable item with the attribute container.
 * @param name The name of the attribute.
 * @return The attribute value.
 * @throw RuntimeException if the attribute doesn't exist or has no value (not set or text).
 */
mvvm::variant_t GetAttributeValue(const mvvm::SessionItem& item, const std::string& name);

/**
 * @brief Removes the value and the text of the attribute, the domain default applies.
 *
 * @param item The instruction or variable item with the attribute container.
 * @param name The name of the attribute.
 * @throw RuntimeException if the attribute doesn't exist or is not nullable.
 */
void UnsetAttributeValue(mvvm::SessionItem& item, const std::string& name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_ATTRIBUTE_CONTAINER_HELPER_H_
