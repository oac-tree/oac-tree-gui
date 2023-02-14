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

#include "sequencergui/model/application_models.h"

#include <gtest/gtest.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>

using namespace sequencergui;

//! Tests of ApplicationModels class.

class ApplicationModelsTest : public ::testing::Test
{
};

TEST_F(ApplicationModelsTest, InitialState)
{
  ApplicationModels models;

  EXPECT_NE(models.GetSequencerModel(), nullptr);
  EXPECT_NE(models.GetJobModel(), nullptr);
  std::vector<mvvm::ApplicationModel*> expected({models.GetSequencerModel(), models.GetJobModel()});
  EXPECT_EQ(models.GetModels(), expected);
}

TEST_F(ApplicationModelsTest, FindItems)
{
  ApplicationModels models;

  // default catalogue is capable of creating sequencer items
  auto procedure = models.GetSequencerModel()->InsertItem<ProcedureItem>();
  EXPECT_EQ(procedure->GetType(), ProcedureItem::Type);

  // second model can find alien item
  EXPECT_EQ(models.GetJobModel()->FindItem(procedure->GetIdentifier()), procedure);
}
