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

namespace oac_tree_gui
{

std::vector<std::string> GetDefaultPluginDirList()
{
  if (sup::gui::IsOnCodac())
  {
    return std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"});
  }

  // FIXME for non-CODAC consider using CMAKE_INSTALL_PREFIX
  return {};
}

}  // namespace oac_tree_gui
