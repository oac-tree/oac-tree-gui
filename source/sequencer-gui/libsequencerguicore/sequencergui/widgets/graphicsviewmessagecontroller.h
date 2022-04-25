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

#ifndef SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H
#define SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H

#include <QObject>
#include <memory>

class QGraphicsView;

namespace sequencergui
{

class OverlayMessage;

//! Controls appearance of overlay messages on top of graphics view.
//! Current implementation is simplified and allows only single message on the screen.

class GraphicsViewMessageController : public QObject
{
  Q_OBJECT

public:
  explicit GraphicsViewMessageController(QGraphicsView* view);
  ~GraphicsViewMessageController() override;

  void AddMessage(const QString& text);

private:
  QGraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<OverlayMessage> m_message;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H
