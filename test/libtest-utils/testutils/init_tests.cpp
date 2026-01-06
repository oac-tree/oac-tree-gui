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

#include "init_tests.h"

#include <oac_tree_gui/components/load_resources.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/domain_library_loader.h>

namespace oac_tree_gui::test
{

void InitTests(int argc, char **argv)
{
  RegisterCustomMetaTypes();

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  DomainLibraryLoader loader(GetBasicPluginFileNames());

  LoadOacTreeItems();
}

}  // namespace oac_tree_gui::test
