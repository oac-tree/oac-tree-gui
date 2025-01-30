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

#include "oac-tree-gui/viewmodel/viewmodel_helper.h"

//! @file
//! Testing helper methods from viewmodel_helper.h

#include <gtest/gtest.h>

#include <QStandardItemModel>
#include <memory>

namespace oac_tree_gui::test
{

class ViewmodelHelperTest : public ::testing::Test
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
  };

  static QList<QStandardItem*> CreateItemRow(const QString& name1, const QString& name2 = {})
  {
    QList<QStandardItem*> result = {new QStandardItem(name1), new QStandardItem(name2)};
    result.at(0)->setData(name1, Qt::EditRole);
    return result;
  }

  /**
   * @brief Creates test model with several branches
   *
   * item0
   *   item1
   *   item2
   *     item3
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

    return data;
  }
};

TEST_F(ViewmodelHelperTest, IterateFirstColumn)
{
  auto data = std::move(CreateTestData());

  std::vector<QModelIndex> visited_indices;
  auto on_index = [&visited_indices](const auto& index) { visited_indices.push_back(index); };

  IterateFirstColumn(*data.model, QModelIndex(), on_index);

  EXPECT_EQ(QModelIndex{}, data.item0->index());

  const std::vector<QModelIndex> expected_indices = {data.item0->index(), data.item1->index(),
                                                     data.item2->index(), data.item3->index()};

  EXPECT_EQ(visited_indices, expected_indices);
}

}
