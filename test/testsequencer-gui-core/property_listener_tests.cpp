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

#include "sequencergui/model/property_listener.h"

#include <mvvm/model/application_model.h>
#include <sequencergui/model/job_item.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing PropertyListener class.

class PropertyListenerTest : public ::testing::Test
{
};

TEST_F(PropertyListenerTest, InitialState)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<JobItem>();

  PropertyListener<JobItem> listener(item);

}
