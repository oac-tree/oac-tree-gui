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

#ifndef MOCKMESSAGEREPORTER_H
#define MOCKMESSAGEREPORTER_H

#include "sequencergui/core/messageinterface.h"

#include <gmock/gmock.h>

#include <string>

//! Convenience class which creates a directory on disk for test content.

class MockMessageReporter : public sequencergui::MessageInterface
{
public:
  MOCK_METHOD1(SendMessage, void(std::string));
};

#endif  //  MOCKMESSAGEREPORTER_H
