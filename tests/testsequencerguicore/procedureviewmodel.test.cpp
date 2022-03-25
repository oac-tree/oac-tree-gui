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

#include "sequencergui/monitor/procedureviewmodel.h"

#include "sequencergui/model/item_constants.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/applicationmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for items from instructionitems.h

class ProcedureViewModelTest : public ::testing::Test
{
public:
  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel") { RegisterItem<ProcedureItem>(); }
  };
};

TEST_F(ProcedureViewModelTest, SingleInstruction)
{
  TestModel model;

  auto item = model.InsertItem<ProcedureItem>();
  item->SetStatus("abc");

  ProcedureViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto displayname_index = viewmodel.index(0, 0);
  auto status_index = viewmodel.index(0, 1);

  auto views = viewmodel.FindViews(item->GetItem(ItemConstants::kStatus));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), status_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(status_index), item->GetItem(ItemConstants::kStatus));

  EXPECT_EQ(viewmodel.data(displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Procedure"));
  EXPECT_EQ(viewmodel.data(status_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
}

TEST_F(ProcedureViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto procedure0 = model.InsertItem<ProcedureItem>();
  auto procedure1 = model.InsertItem<ProcedureItem>();

  ProcedureViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto status_index = viewmodel.index(0, 1);

  QSignalSpy spy_data_changed(&viewmodel, &ProcedureViewModel::dataChanged);

  procedure0->SetStatus("abc");
  EXPECT_EQ(spy_data_changed.count(), 1);
}
