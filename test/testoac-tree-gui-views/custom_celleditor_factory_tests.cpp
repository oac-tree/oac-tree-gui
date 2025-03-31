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

#include "oac_tree_gui/views/composer/custom_celleditor_factory.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui
{

class CustomCellEditorFactoryTest : public ::testing::Test
{
public:
private:
};

TEST_F(CustomCellEditorFactoryTest, CustomCellEditorFactory)
{
  CustomCellEditorFactory factory({});
}

}  // namespace oac_tree_gui
