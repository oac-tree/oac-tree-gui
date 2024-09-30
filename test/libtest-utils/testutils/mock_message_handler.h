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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_MESSAGE_HANDLER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_MESSAGE_HANDLER_H_

#include <sup/gui/core/message_handler_interface.h>

#include <gmock/gmock.h>

#include <memory>

namespace testutils
{

//! Mock class to use as MessageHandler.

class MockMessageHandler : public sup::gui::MessageHandlerInterface
{
public:
  MOCK_METHOD(void, SendMessage, (const sup::gui::MessageEvent&));
};

//! Create decorator around MockMessageHandler.
//! This is to avoid pasing unique_ptr<MockMessageHandler> directly, since it triggers
//! googletest warnings related to testing::Mock::AllowLeak.

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandlerDecorator(
    MockMessageHandler* mock_handler);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_MESSAGE_HANDLER_H_
