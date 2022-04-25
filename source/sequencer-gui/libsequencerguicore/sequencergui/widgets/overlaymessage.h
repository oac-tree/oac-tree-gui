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

#ifndef SEQUENCERGUI_WIDGETS_OVERLAYMESSAGE_H
#define SEQUENCERGUI_WIDGETS_OVERLAYMESSAGE_H

class QWidget;
class QString;

#include <memory>

namespace sequencergui
{

class OverlayMessageFrame;
class OverlayMessageController;

//! Represents a message on top of scroll area.

class OverlayMessage
{
public:
  explicit OverlayMessage(const QString& text, QWidget* area);
  ~OverlayMessage();

  bool CanBeDeleted();

private:
  std::unique_ptr<QWidget> m_message_widget;
  std::unique_ptr<OverlayMessageController> m_message_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_OVERLAYMESSAGE_H
