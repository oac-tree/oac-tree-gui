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

#include "oac_tree_gui/model/standard_job_items.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

namespace oac_tree_gui
{

/**
 * @brief Tests for items from standard_job_items.h.
 */
class StandardJobItemsTest : public ::testing::Test
{
};

TEST_F(StandardJobItemsTest, RemoteJobItem)
{
  RemoteJobItem item;

  EXPECT_EQ(item.GetRemoteJobIndex(), 0);
  EXPECT_EQ(item.GetServerName(), std::string());

  item.SetRemoteJobIndex(42);
  EXPECT_EQ(item.GetRemoteJobIndex(), 42);

  item.SetServerName("abc");
  EXPECT_EQ(item.GetServerName(), std::string("abc"));
}

TEST_F(StandardJobItemsTest, FileBasedJobItem)
{
  FileBasedJobItem item;

  EXPECT_TRUE(item.GetFileName().empty());

  item.SetFileName("abc");
  EXPECT_EQ(item.GetFileName(), std::string("abc"));
}

TEST_F(StandardJobItemsTest, CreateLocalJobItem)
{
  EXPECT_THROW(CreateLocalJobItem(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto procedure = model.InsertItem<ProcedureItem>();
  procedure->SetDisplayName("abc");

  auto job_item = CreateLocalJobItem(procedure, 42);
  auto job_item_ptr = job_item.get();

  EXPECT_EQ(job_item->GetTickTimeout(), 42);
  EXPECT_EQ(job_item->GetDisplayName(), std::string("abc"));

  // linked mechanism will be working only after JobItem become part of the model
  EXPECT_EQ(job_item->GetProcedure(), nullptr);

  model.InsertItem(std::move(job_item), model.GetRootItem(), mvvm::TagIndex::Append());
  EXPECT_EQ(job_item_ptr->GetProcedure(), procedure);
}

TEST_F(StandardJobItemsTest, CreateImportedJobItem)
{
  EXPECT_THROW(CreateImportedJobItem(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  procedure->SetDisplayName("abc");

  auto job_item = CreateImportedJobItem(std::move(procedure), 42);
  auto job_item_ptr = job_item.get();

  EXPECT_EQ(job_item->GetTickTimeout(), 42);
  EXPECT_EQ(job_item->GetDisplayName(), std::string("abc"));

  // linked mechanism will be working only after JobItem become part of the model
  EXPECT_EQ(job_item->GetProcedure(), nullptr);
  model.InsertItem(std::move(job_item), model.GetRootItem(), mvvm::TagIndex::Append());
  EXPECT_EQ(job_item_ptr->GetProcedure(), procedure_ptr);

  // imported procedure is stored on-board
  EXPECT_EQ(job_item_ptr->GetItem(mvvm::TagIndex::First(ImportedJobItem::kImportedProcedure)),
            procedure_ptr);
}

TEST_F(StandardJobItemsTest, CreateRemoteJobItem)
{
  const std::string server_name("abc");
  const std::size_t job_index{42};

  auto item = CreateRemoteJobItem(server_name, job_index);

  EXPECT_EQ(item->GetRemoteJobIndex(), job_index);
  EXPECT_EQ(item->GetServerName(), server_name);
  EXPECT_EQ(item->GetDisplayName(), "abc_42");
}

TEST_F(StandardJobItemsTest, CreateFileBasedJobItem)
{
  const std::string file_name("abc");

  auto item = CreateFileBasedJobItem(file_name, 42);

  EXPECT_EQ(item->GetTickTimeout(), 42);
  EXPECT_EQ(item->GetProcedure(), nullptr);
  EXPECT_EQ(item->GetDisplayName(), FileBasedJobItem::GetStaticType());

  auto file_based_item = dynamic_cast<FileBasedJobItem*>(item.get());
  ASSERT_NE(file_based_item, nullptr);
  EXPECT_EQ(file_based_item->GetFileName(), std::string("abc"));
}

}  // namespace oac_tree_gui
