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

#include "mock_message_handler.h"

#include <sup/gui/components/message_handler_decorator.h>

namespace testutils
{

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandlerDecorator(
    MockMessageHandler *mock_handler)
{
  return sup::gui::MessageHandlerDecorator::Create(mock_handler);
}

}  // namespace testutils
