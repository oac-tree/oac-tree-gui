/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "mock_message_handler.h"

#include <sup/gui/core/message_handler_decorator.h>

namespace testutils
{

TestMessageHandler::TestMessageHandler(std::function<void(const sup::gui::MessageEvent &)> callback)
    : m_callback(std::move(callback))
{
}

void TestMessageHandler::SendMessage(const sup::gui::MessageEvent &message)
{
  if (m_callback)
  {
    m_callback(message);
  }
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateTestMessageHandler(
    std::function<void(const sup::gui::MessageEvent &)> callback)
{
  return std::make_unique<TestMessageHandler>(std::move(callback));
}

}  // namespace testutils
