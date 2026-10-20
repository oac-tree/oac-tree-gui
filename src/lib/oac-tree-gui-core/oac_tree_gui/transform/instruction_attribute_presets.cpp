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

#include "instruction_attribute_presets.h"

#include <oac_tree_gui/domain/domain_constants.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the preset key for the given instruction type and attribute.
std::string GetPresetKey(const std::string& domain_type, const std::string& attribute_name)
{
  return domain_type + "/" + attribute_name;
}

}  // namespace

const std::map<std::string, AttributePreset>& GetAttributePresets()
{
  static const std::map<std::string, AttributePreset> kPresets = {
      {GetPresetKey(domainconstants::kRepeatInstructionType, domainconstants::kMaxCountAttribute),
       {mvvm::int32{-1}}}};
  return kPresets;
}

std::optional<AttributePreset> FindAttributePreset(const std::string& domain_type,
                                                   const std::string& attribute_name)
{
  const auto& presets = GetAttributePresets();
  if (auto iter = presets.find(GetPresetKey(domain_type, attribute_name)); iter != presets.end())
  {
    return iter->second;
  }
  return std::nullopt;
}

}  // namespace oac_tree_gui
