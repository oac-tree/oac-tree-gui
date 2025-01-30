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

#include <oac_tree_gui/model/procedure_item.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/model/job_model.h"

namespace oac_tree_gui::test
{

//! Tests for JobModel class.

class JobModelTest : public ::testing::Test
{
};

TEST_F(JobModelTest, InitialState)
{
  JobModel model;

  // trying to insert procedure to make sure that catalogue is there
  auto item = model.InsertItem<ProcedureItem>();
  EXPECT_EQ(item->GetType(), ProcedureItem::GetStaticType());
  EXPECT_NE(dynamic_cast<ProcedureItem*>(item), nullptr);
}

}  // namespace oac_tree_gui::test
