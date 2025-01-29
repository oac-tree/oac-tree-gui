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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac-tree-gui/components/custom_meta_types.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/register_items.h>

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  sequencergui::LoadPlugins();
  sequencergui::RegisterCustomMetaTypes();
  sequencergui::RegisterSessionItems();

  // run all google tests
  return RUN_ALL_TESTS();
}
