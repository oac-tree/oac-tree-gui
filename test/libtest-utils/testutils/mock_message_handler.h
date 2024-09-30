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

#include <functional>
#include <memory>

namespace testutils
{

/**
 * @brief The TestMessageHandler class is a test message handler which calls external callback on
 * own method call.
 */
class TestMessageHandler : public sup::gui::MessageHandlerInterface
{
public:
  explicit TestMessageHandler(std::function<void(const sup::gui::MessageEvent&)> callback);

  void SendMessage(const sup::gui::MessageEvent& message) override;

private:
  std::function<void(const sup::gui::MessageEvent&)> m_callback;
};

/**
 * @brief Creates TestMessageHandler which will use given callback.
 */
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateTestMessageHandler(
    std::function<void(const sup::gui::MessageEvent&)> callback);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_MESSAGE_HANDLER_H_
