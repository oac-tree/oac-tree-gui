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

#include "sequencer_settings_helper.h"

#include <sup/gui/core/environment.h>

#include <mvvm/core/platform.h>
#include <mvvm/utils/file_utils.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kSharedLibLinuxExtension = ".so";
constexpr auto kSharedLibMacExtension = ".dylib";
}  // namespace

std::vector<std::string> GetDefaultPluginDirList()
{
  if (sup::gui::IsOnCodac())
  {
    return std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"});
  }

  // FIXME for non-CODAC consider using CMAKE_INSTALL_PREFIX
  return {};
}

std::vector<std::string> FindSharedLibraries(const std::string &dir)
{
  std::vector<std::string> result;

  const auto so_names = mvvm::utils::FindFiles(dir, kSharedLibLinuxExtension);
  const auto dylib_names = mvvm::utils::FindFiles(dir, kSharedLibMacExtension);

  result.reserve(so_names.size() + dylib_names.size());
  result.insert(result.end(), so_names.begin(), so_names.end());
  result.insert(result.end(), dylib_names.begin(), dylib_names.end());

  return result;
}

}  // namespace oac_tree_gui
