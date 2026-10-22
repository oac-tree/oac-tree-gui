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

#ifndef OAC_TREE_GUI_DOMAIN_ATTRIBUTE_DESCRIPTOR_H_
#define OAC_TREE_GUI_DOMAIN_ATTRIBUTE_DESCRIPTOR_H_

//! @file
//! Description of a domain attribute, used to build attribute items.

#include <mvvm/core/variant.h>

#include <sup/oac-tree/attribute_definition.h>

#include <string>

namespace oac_tree_gui
{

/**
 * @brief Returns true if the given variant holds a string.
 */
inline bool IsString(const mvvm::variant_t& value)
{
  return std::holds_alternative<std::string>(value);
}

/**
 * @brief The AttributeDescriptor struct describes a domain attribute of an instruction or a
 * variable type.
 */
struct AttributeDescriptor
{
  std::string name;
  mvvm::variant_t default_value;  //!< scalar; its type defines the attribute type
  std::string tooltip;
  bool mandatory{false};
  bool has_preset{false};  //!< nullable attribute that starts with the default value
  sup::oac_tree::AttributeCategory category{sup::oac_tree::AttributeCategory::kLiteral};
  bool visible{true};     //!< shown in attribute editor and tooltips
  bool allow_text{true};  //!< text (`$par`, `@var`) accepted instead of a value
  bool nullable{true};    //!< can have no value; false for mandatory, isRoot and strings
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_ATTRIBUTE_DESCRIPTOR_H_
