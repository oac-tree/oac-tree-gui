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

#include "sequencergui/model/job_model.h"

#include <sequencergui/model/procedure_item.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobModel class.

class JobModelTest : public ::testing::Test
{
};

TEST_F(JobModelTest, InitialState)
{
  JobModel model;

  // trying to insert procedure to make sure that catalogue is there
  auto item = model.InsertNewItem(ProcedureItem::Type);
  EXPECT_EQ(item->GetType(), ProcedureItem::Type);
  EXPECT_NE(dynamic_cast<ProcedureItem*>(item), nullptr);
}
