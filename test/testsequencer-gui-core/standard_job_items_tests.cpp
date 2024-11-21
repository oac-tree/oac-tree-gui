/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standard_job_items.h"

#include <sequencergui/model/procedure_item.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

namespace sequencergui
{

/**
 * @brief Tests for items from standard_job_items.h.
 */
class StandardJobItemsTest : public ::testing::Test
{
};

TEST_F(StandardJobItemsTest, ImportedJobItem) {}

TEST_F(StandardJobItemsTest, RemoteJobItem)
{
  RemoteJobItem item;

  EXPECT_EQ(item.GetRemoteJobIndex(), -1);
  EXPECT_EQ(item.GetServerName(), std::string());

  item.SetRemoteJobIndex(42);
  EXPECT_EQ(item.GetRemoteJobIndex(), 42);

  item.SetServerName("abc");
  EXPECT_EQ(item.GetServerName(), std::string("abc"));
}

TEST_F(StandardJobItemsTest, CreateLocalJobItem)
{
  mvvm::ApplicationModel model;
  auto procedure = model.InsertItem<ProcedureItem>();

  auto job_item = CreateLocalJobItem(procedure);
  auto job_item_ptr = job_item.get();

  // linked mechanism will be working only after JobItem become part of the model
  EXPECT_EQ(job_item->GetProcedure(), nullptr);

  model.InsertItem(std::move(job_item), model.GetRootItem(), mvvm::TagIndex::Append());
  EXPECT_EQ(job_item_ptr->GetProcedure(), procedure);
}

TEST_F(StandardJobItemsTest, CreateImportedJobItem)
{
  mvvm::ApplicationModel model;

  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();

  auto job_item = CreateImportedJobItem(std::move(procedure));
  auto job_item_ptr = job_item.get();

  // linked mechanism will be working only after JobItem become part of the model
  EXPECT_EQ(job_item->GetProcedure(), nullptr);
  model.InsertItem(std::move(job_item), model.GetRootItem(), mvvm::TagIndex::Append());
  EXPECT_EQ(job_item_ptr->GetProcedure(), procedure_ptr);

  // imported procedure is stored on-board
  EXPECT_EQ(job_item_ptr->GetItem(mvvm::TagIndex::First(ImportedJobItem::kImportedProcedure)),
            procedure_ptr);
}

}  // namespace sequencergui
