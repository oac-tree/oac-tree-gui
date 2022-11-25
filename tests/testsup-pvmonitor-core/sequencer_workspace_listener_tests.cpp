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

#include "suppvmonitor/sequencer_workspace_listener.h"

#include <gtest/gtest.h>
#include <sequencergui/core/exceptions.h>
#include <sup/sequencer/workspace.h>

using namespace suppvmonitor;

//! Tests for InstructionViewItemMap class.

class SequencerWorkspaceListenerTests : public ::testing::Test
{
};

//! Checking exceptions during start/stop listening.

TEST_F(SequencerWorkspaceListenerTests, StartListeningStopListening)
{
  SequencerWorkspaceListener listener;

  EXPECT_THROW(listener.StartListening(nullptr), sequencergui::RuntimeException);
  EXPECT_THROW(listener.StopListening(), sequencergui::RuntimeException);

  sup::sequencer::Workspace workspace;

  // it is possible to start listening to the given workspace only once
  EXPECT_NO_THROW(listener.StartListening(&workspace));
  EXPECT_THROW(listener.StartListening(nullptr), sequencergui::RuntimeException);

  // it is possible to stop listening given workspace only once
  EXPECT_NO_THROW(listener.StopListening());
  EXPECT_THROW(listener.StopListening(), sequencergui::RuntimeException);
}
