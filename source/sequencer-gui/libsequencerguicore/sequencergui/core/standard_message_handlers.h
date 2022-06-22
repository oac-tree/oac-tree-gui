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

#ifndef SEQUENCERGUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
#define SEQUENCERGUI_CORE_STANDARD_MESSAGE_HANDLERS_H_

#include <sequencergui/core/message_handler_interface.h>

namespace sequencergui
{

//! Simple message handler that prints messages to standard output.

class StdMessageHandler : public MessageHandlerInterface
{
public:
  void SendMessage(const std::string& text) override;
};

//! Create null message handlers. It simply throws the message.

class NullMessageHandler : public MessageHandlerInterface
{
public:
  void SendMessage(const std::string& text) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
