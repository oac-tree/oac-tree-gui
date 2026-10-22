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

#include "variable_descriptor_helper.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/transform/attribute_descriptor_helper.h>

#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

namespace oac_tree_gui
{

VariableDescriptor CreateVariableDescriptor(const variable_t& variable)
{
  const auto domain_type = variable.GetType();
  VariableDescriptor result{domain_type, {}};
  for (const auto& definition : variable.GetAttributeDefinitions())
  {
    auto attribute = CreateAttributeDescriptor(domain_type, definition);
    attribute.allow_text = false;  // variable attributes take no text
    result.attributes.push_back(attribute);
  }
  return result;
}

VariableDescriptor CreateVariableDescriptor(const sup::oac_tree::VariableInfo& info)
{
  return {info.GetType(), {}};
}

VariableNode CreateVariableNode(const std::string& domain_type)
{
  return {CreateVariableDescriptor(*CreateDomainVariable(domain_type)), {}, 0};
}

VariableNode CreateVariableNode(const variable_t& variable)
{
  return {CreateVariableDescriptor(variable), variable.GetStringAttributes(), 0};
}

VariableNode CreateVariableNode(const sup::oac_tree::VariableInfo& info)
{
  return {CreateVariableDescriptor(info), info.GetAttributes(), info.GetIndex()};
}

}  // namespace oac_tree_gui
