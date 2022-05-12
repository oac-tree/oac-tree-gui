/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/components/itemviewcomponentprovider.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <gtest/gtest.h>

#include <QTreeView>

using namespace sequencergui;

class ItemViewComponentProviderTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

TEST_F(ItemViewComponentProviderTest, InitialState)
{
  QTreeView view;

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_EQ(provider.GetViewModel(), nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());
}
