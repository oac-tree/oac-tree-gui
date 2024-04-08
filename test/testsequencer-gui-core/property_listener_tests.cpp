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
#include <sequencergui/model/item_constants.h>

#include <gtest/gtest.h>
#include <mvvm/test/mock_callback_listener.h>

using namespace sequencergui;

//! Testing PropertyListener class.

class PropertyListenerTest : public ::testing::Test
{
public:
  mvvm::test::MockCallbackListener<mvvm::event_variant_t> m_event_listener;
};

TEST_F(PropertyListenerTest, InitialState)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<JobItem>();

  PropertyListener<JobItem> listener(item);

  listener.Connect<mvvm::PropertyChangedEvent>(m_event_listener.CreateCallback());

  mvvm::PropertyChangedEvent expected_event{item, itemconstants::kTickTimeout};

  EXPECT_CALL(m_event_listener, OnCallback(mvvm::event_variant_t{expected_event})).Times(1);

  item->SetTickTimeout(42);
}
