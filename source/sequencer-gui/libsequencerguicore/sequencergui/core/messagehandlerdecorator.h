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

#ifndef SEQUENCERGUI_CORE_MESSAGEHANDLERDECORATOR_H
#define SEQUENCERGUI_CORE_MESSAGEHANDLERDECORATOR_H

#include "sequencergui/core/messagehandlerinterface.h"

#include <memory>

namespace sequencergui
{

//! Decorator for message handler. Used when we have to use MessageHandler with unique_ptr but still
//! don't want to pass ownership.

class MessageHandlerDecorator : public MessageHandlerInterface
{
public:
  explicit MessageHandlerDecorator(MessageHandlerInterface* component);

  static std::unique_ptr<MessageHandlerInterface> Create(MessageHandlerInterface* component);

  void SendMessage(const std::string& text) override;

private:
  MessageHandlerInterface* m_component{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_MESSAGEHANDLERDECORATOR_H
