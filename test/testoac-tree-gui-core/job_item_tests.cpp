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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/job_item.h"

#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <gtest/gtest.h>

namespace
{
class TestItem : public oac_tree_gui::JobItem
{
public:
  TestItem() : JobItem("TestItem") {}

  std::unique_ptr<SessionItem> Clone() const override { return std::make_unique<TestItem>(*this); }
};

}  // namespace

template <>
struct mvvm::item_traits<TestItem>
{
  static constexpr std::string_view type_name() noexcept { return "TestItem"; }
};

namespace oac_tree_gui::test
{

//! Tests for JobItem class.
class JobItemTest : public ::testing::Test
{
};

TEST_F(JobItemTest, JobItem)
{
  TestItem item;
  EXPECT_EQ(item.GetStatus(), RunnerStatus::kUndefined);

  EXPECT_EQ(item.GetTickTimeout(),
            std::chrono::milliseconds{itemconstants::kDefaultTickTimeoutMsec});
  item.SetTickTimeout(std::chrono::milliseconds{42});
  EXPECT_EQ(item.GetTickTimeout(), std::chrono::milliseconds{42});

  item.SetStatus(RunnerStatus::kInitial);
  EXPECT_EQ(item.GetStatus(), RunnerStatus::kInitial);

  EXPECT_EQ(item.GetExpandedProcedure(), nullptr);
}

TEST_F(JobItemTest, SetProcedure)
{
  SequencerModel model;

  auto job_item = model.InsertItem<TestItem>();
  auto procedure_item = model.InsertItem<ProcedureItem>();

  // checking JobItem search machinery
  job_item->SetProcedure(procedure_item);
  EXPECT_EQ(job_item->GetProcedure(), procedure_item);
}

//! Checking that JobItem can find linked rocedure when they both belong to different models.

TEST_F(JobItemTest, SetProcedureInterModelContext)
{
  ApplicationModels models;
  models.CreateEmpty();

  auto job_item = models.GetJobModel()->InsertItem<TestItem>();
  auto procedure_item = models.GetSequencerModel()->InsertItem<ProcedureItem>();

  // checking JobItem search machinery
  job_item->SetProcedure(procedure_item);
  EXPECT_EQ(job_item->GetProcedure(), procedure_item);
}

}  // namespace oac_tree_gui::test
