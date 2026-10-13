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

#ifndef OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ATTRIBUTE_PRESETS_H_
#define OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ATTRIBUTE_PRESETS_H_

//! @file
//! Initial values of instruction attributes, until the domain provides default values.

#include <mvvm/core/variant.h>

#include <map>
#include <optional>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The AttributePreset struct holds the initial value of a domain attribute.
 */
struct AttributePreset
{
  mvvm::variant_t value;
  bool exposed{true};  //!< the attribute goes to the domain
};

/**
 * @brief Returns attribute presets.
 *
 * The key is "<DomainType>/<attribute>".
 */
const std::map<std::string, AttributePreset>& GetAttributePresets();

/**
 * @brief Finds the preset of the given attribute of the given instruction type.
 *
 * @param domain_type The domain type of the instruction.
 * @param attribute_name The name of the attribute.
 * @return The preset, or nullopt if no preset exists.
 */
std::optional<AttributePreset> FindAttributePreset(const std::string& domain_type,
                                                   const std::string& attribute_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ATTRIBUTE_PRESETS_H_
