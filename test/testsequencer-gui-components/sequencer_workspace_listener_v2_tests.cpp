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

#include "sequencergui/pvmonitor/sequencer_workspace_listener_v2.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/pvmonitor/workspace_event.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for SequencerWorkspaceListenerV2 class.
class SequencerWorkspaceListenerV2Test : public ::testing::Test
{
};

//! Initial state.
TEST_F(SequencerWorkspaceListenerV2Test, InitialState)
{
}
