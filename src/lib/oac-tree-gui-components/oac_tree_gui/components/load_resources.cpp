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

#include "load_resources.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/register_items.h>

#include <sup/gui/model/register_items.h>

namespace oac_tree_gui
{

void LoadOacTreeItems()
{
  sup::gui::RegisterSessionItems();
  oac_tree_gui::RegisterSessionItems();
}

void LoadOacTreeDomainPlugins()
{
  (void) LoadOacTreePlugins();
}

}  // namespace oac_tree_gui
