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

#include "sequencergui/model/job_item.h"

#include <sequencergui/model/application_models.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobItem class.

class JobItemTest : public ::testing::Test
{
public:
  class TestItem : public JobItem
  {
  public:
    TestItem() : JobItem("TestItem") {}
  };
};

TEST_F(JobItemTest, JobItem)
{
  TestItem item;

  EXPECT_EQ(item.GetTickTimeout(), itemconstants::kDefaultTickTimeoutMsec);
  item.SetTickTimeout(42);
  EXPECT_EQ(item.GetTickTimeout(), 42);

  item.SetStatus("abc");
  EXPECT_EQ(item.GetStatus(), "abc");

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
