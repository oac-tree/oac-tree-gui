/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "context_helper.h"

#include "sequencer_main_window_context.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/domain_object_group_helper.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>

#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace oac_tree_gui
{

namespace
{
/**
 * @brief Returns vector of instruction names from the list containing a mixture of different names.
 */
std::vector<std::string> GetInstructionNames(const std::vector<std::string>& names)
{
  std::vector<std::string> result;
  const static auto known_types = oac_tree_gui::GetDomainInstructionNames();

  auto on_element = [](auto element) { return mvvm::utils::Contains(known_types, element); };
  std::copy_if(std::begin(names), std::end(names), std::back_inserter(result), on_element);
  return result;
}
}  // namespace

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups()
{
  auto context = FindSequencerMainWindowContext();
  if (context == nullptr)
  {
    throw RuntimeException("SequencerMainWindowContext is not found");
  }

  auto& type_registry = context->GetObjectTypeRegistry();

  // Global type registry contains both instruction and variables. We have to remove variable names
  // before returning info to the user.

  auto group_infos = CreateInstructionTypeGroups(type_registry);

  for (auto& info : group_infos)
  {
    info.object_names = GetInstructionNames(info.object_names);
  }
  return group_infos;
}

}  // namespace oac_tree_gui
