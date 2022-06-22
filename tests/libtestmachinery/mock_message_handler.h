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

#ifndef MOCKMESSAGEHANDLER_H
#define MOCKMESSAGEHANDLER_H

#include <sequencergui/core/message_handler_interface.h>

#include <gmock/gmock.h>

#include <memory>
#include <string>

//! Mock class to use as MessageHandler.

class MockMessageHandler : public sequencergui::MessageHandlerInterface
{
public:
  MOCK_METHOD1(SendMessage, void(const std::string&));
};

//! Create decorator around MockMessageHandler.
//! This is to avoid pasing unique_ptr<MockMessageHandler> directly, since it triggers
//! googletest warnings related to testing::Mock::AllowLeak.

std::unique_ptr<sequencergui::MessageHandlerInterface> CreateMessageHandlerDecorator(
    MockMessageHandler* mock_handler);

#endif  //  MOCKMESSAGEHANDLER_H
