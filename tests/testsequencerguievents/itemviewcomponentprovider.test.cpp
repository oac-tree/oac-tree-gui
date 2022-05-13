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
#include "mvvm/model/compounditem.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QTreeView>

using namespace sequencergui;

class ItemViewComponentProviderTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

//! Initial state when no ApplicationModel is set.

TEST_F(ItemViewComponentProviderTest, InitialState)
{
  QTreeView view;

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_EQ(provider.GetSelectionModel()->model(), nullptr);
  EXPECT_EQ(provider.GetViewModel(), nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());
}

//! Setting empty ApplicationModel to the provider.

TEST_F(ItemViewComponentProviderTest, SetEmptyModel)
{
  QTreeView view;

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);
  provider.SetApplicationModel(&m_model);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_NE(provider.GetViewModel(), nullptr);
  EXPECT_NE(dynamic_cast<mvvm::AllItemsViewModel*>(provider.GetViewModel()), nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());

  EXPECT_EQ(provider.GetView()->model(), provider.GetViewModel());
  EXPECT_EQ(provider.GetSelectionModel()->model(), provider.GetViewModel());
}

//! Setting ApplicationModel with single item to the provider.

TEST_F(ItemViewComponentProviderTest, SetNonEmptyModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);
  provider.SetApplicationModel(&m_model);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);

  auto viewmodel = provider.GetViewModel();

  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);
}

//! No model initialisation, setting an item.

TEST_F(ItemViewComponentProviderTest, SetItem)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::VectorItem>();

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);

  provider.SetItem(item);

  auto viewmodel = provider.GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(), 3);  // x, y, z
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_EQ(provider.GetView()->model(), viewmodel);
  EXPECT_EQ(provider.GetSelectionModel()->model(), provider.GetViewModel());
}

//! Initialising provider with application model, then changing root item.

TEST_F(ItemViewComponentProviderTest, SetItemAfterSetModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);
  provider.SetApplicationModel(&m_model);

  auto viewmodel = provider.GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_EQ(provider.GetView()->model(), viewmodel);
  EXPECT_EQ(provider.GetSelectionModel()->model(), provider.GetViewModel());

  // setting new item as root item
  provider.SetItem(item);

  auto new_viewmodel = provider.GetViewModel();

  // in the current implementation of ItemViewComponentProvider, setting a new item will regenerate
  // a view model
  EXPECT_NE(viewmodel, new_viewmodel);
  EXPECT_EQ(provider.GetView()->model(), new_viewmodel);
  EXPECT_EQ(provider.GetSelectionModel()->model(), new_viewmodel);

  EXPECT_EQ(new_viewmodel->GetRootSessionItem(), item);

  EXPECT_EQ(provider.GetSelectedItem(), nullptr);

  // no rows and columns since our item plays the role of root item
  EXPECT_EQ(new_viewmodel->rowCount(), 0);
  EXPECT_EQ(new_viewmodel->columnCount(), 0);
}

//! Attempt to set one item after another, when they belongs to different models
//! (real life bug)

TEST_F(ItemViewComponentProviderTest, SetItemAfterItem)
{
  QTreeView view;

  mvvm::ApplicationModel model1;
  mvvm::ApplicationModel model2;

  auto item1 = model1.InsertItem<mvvm::CompoundItem>();
  item1->AddBranch<mvvm::VectorItem>("position");

  auto item2 = model2.InsertItem<mvvm::CompoundItem>();

  ItemViewComponentProvider provider(CreateViewModel<mvvm::AllItemsViewModel>, &view);

  // setting item from the first model
  provider.SetItem(item1);

  EXPECT_EQ(provider.GetViewModel()->GetRootSessionItem(), item1);
  EXPECT_EQ(provider.GetViewModel()->rowCount(), 1);

  // setting item from the second model
  EXPECT_NO_THROW(provider.SetItem(item2));
  EXPECT_EQ(provider.GetViewModel()->rowCount(), 0);
}
