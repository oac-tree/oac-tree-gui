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

#include "sequencergui/model/job_item.h"

#include "sequencergui/model/application_models.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobItem class.

class JobItemTest : public ::testing::Test
{
};

TEST_F(JobItemTest, JobItem)
{
  JobItem item;

  item.SetStatus("abc");
  EXPECT_EQ(item.GetStatus(), "abc");

  EXPECT_EQ(item.GetExpandedProcedure(), nullptr);
}

TEST_F(JobItemTest, SetProcedure)
{
  SequencerModel model;

  auto job_item = model.InsertItem<JobItem>();
  auto procedure_item = model.InsertItem<ProcedureItem>();

  // checking JobItem search machinery
  job_item->SetProcedure(procedure_item);
  EXPECT_EQ(job_item->GetProcedure(), procedure_item);
}

TEST_F(JobItemTest, SetProcedureInterMoidelContext)
{
  ApplicationModels models;

  auto job_item = models.GetJobModel()->InsertItem<JobItem>();
  auto procedure_item = models.GetSequencerModel()->InsertItem<ProcedureItem>();

  // checking JobItem search machinery
  job_item->SetProcedure(procedure_item);
  EXPECT_EQ(job_item->GetProcedure(), procedure_item);
}
