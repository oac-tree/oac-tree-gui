/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/widgets/tree_helper.h"

//! @file
//! Testing helper methods from tree_helper.h

#include <gtest/gtest.h>

#include <QStandardItemModel>
#include <QTreeView>
#include <memory>

using namespace sequencergui;

class TreeHelperTest : public ::testing::Test
{
public:
  /**
   * @brief The convenience
   */
  struct TestData
  {
    std::unique_ptr<QStandardItemModel> model;
    QStandardItem* item0{nullptr};
    QStandardItem* item1{nullptr};
    QStandardItem* item2{nullptr};
    QStandardItem* item3{nullptr};
    QStandardItem* item4{nullptr};
  };

  static QList<QStandardItem*> CreateItemRow(const QString& name)
  {
    QList<QStandardItem*> result = {new QStandardItem(name)};
    result.at(0)->setData(name, Qt::EditRole);
    return result;
  }

  /**
   * @brief Creates test model with several branches
   *
   * item0
   *   item1
   *   item2
   *     item3
   *     item4
   */
  static TestData CreateTestData()
  {
    TestData data;
    data.model = std::make_unique<QStandardItemModel>();

    data.item0 = data.model->invisibleRootItem();

    auto row = CreateItemRow("item1");
    data.item1 = row.at(0);
    data.item0->insertRow(0, row);

    row = CreateItemRow("item2");
    data.item2 = row.at(0);
    data.item0->insertRow(1, row);

    row = CreateItemRow("item3");
    data.item3 = row.at(0);
    data.item2->insertRow(0, row);

    row = CreateItemRow("item4");
    data.item4 = row.at(0);
    data.item2->insertRow(1, row);

    return data;
  }
};

TEST_F(TreeHelperTest, CheckCollapseExpand)
{
  auto data = std::move(CreateTestData());

  QTreeView tree;
  tree.setModel(data.model.get());

  // by default all is collapsed, even branches which have no children
  EXPECT_FALSE(tree.isExpanded(data.item0->index()));
  EXPECT_FALSE(tree.isExpanded(data.item1->index()));
  EXPECT_FALSE(tree.isExpanded(data.item2->index()));

  // expanding one branch
  tree.setExpanded(data.item2->index(), true);
  EXPECT_FALSE(tree.isExpanded(data.item0->index()));
  EXPECT_FALSE(tree.isExpanded(data.item1->index()));
  EXPECT_TRUE(tree.isExpanded(data.item2->index()));

  // expanding all
  tree.expandAll();
  // seems invisible root always reports himself collapsed
  EXPECT_FALSE(tree.isExpanded(data.item0->index()));
  EXPECT_TRUE(tree.isExpanded(data.item1->index()));
  EXPECT_TRUE(tree.isExpanded(data.item2->index()));
}

TEST_F(TreeHelperTest, FindLastCollapsedParent)
{
  auto data = std::move(CreateTestData());
  QTreeView tree;
  tree.setModel(data.model.get());

  tree.expandAll();

  // for invalid index the result should be invalid too
  EXPECT_EQ(FindVisibleCandidate(tree, QModelIndex()), QModelIndex());

  // for invisible root item result should be invalid
  EXPECT_EQ(FindVisibleCandidate(tree, data.item0->index()), QModelIndex());

  // Since all tree is expanded, the result should be index itself. This means that for fully
  // expanded tree we will select the item as it is.
  EXPECT_EQ(FindVisibleCandidate(tree, data.item1->index()), data.item1->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item2->index()), data.item2->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item3->index()), data.item3->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item4->index()), data.item4->index());

  // collapsing item2
  tree.setExpanded(data.item2->index(), false);
  EXPECT_EQ(FindVisibleCandidate(tree, data.item1->index()), data.item1->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item2->index()), data.item2->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item3->index()), data.item2->index());
  EXPECT_EQ(FindVisibleCandidate(tree, data.item4->index()), data.item2->index());
}
