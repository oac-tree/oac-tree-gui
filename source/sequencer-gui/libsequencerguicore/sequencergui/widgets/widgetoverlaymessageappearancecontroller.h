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

#ifndef SEQUENCERGUI_WIDGETS_WIDGETOVERLAYMESSAGEAPPEARANCECONTROLLER_H
#define SEQUENCERGUI_WIDGETS_WIDGETOVERLAYMESSAGEAPPEARANCECONTROLLER_H

#include <QObject>
#include <memory>

class QWidget;
class QTimer;

namespace sequencergui
{

class OverlayMessage;

//! Controls appearance of overlay messages on top of arbitrary widget.
//! Current implementation is simplified and allows only single message on the screen.

class WidgetOverlayMessageAppearanceController : public QObject
{
  Q_OBJECT

public:
  explicit WidgetOverlayMessageAppearanceController(QWidget* widget);
  ~WidgetOverlayMessageAppearanceController() override;

  void AddMessage(const QString& text);

private:
  void RemoveMessageOnTimeout();

  QWidget* m_widget{nullptr};
  std::unique_ptr<OverlayMessage> m_message;
  QTimer* m_timer{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_WIDGETOVERLAYMESSAGEAPPEARANCECONTROLLER_H
