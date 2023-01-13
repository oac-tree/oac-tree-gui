/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_
#define SEQUENCERGUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_

#include <QFrame>

class QWidget;
class QPoint;

namespace sequencergui
{

class OverlayMessageFrame;

//! Controls the appearance of overlay widget (position, show/hide) on top of another widget.
//! Current implementation places a message box at the lower right corner of the widget (hardcoded
//! behavior).

class OverlayWidgetPositionStrategy : public QObject
{
  Q_OBJECT

public:
  explicit OverlayWidgetPositionStrategy(QWidget* overlay_widget, QWidget* area,
                                         QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  void UpdateMessageWidgetGeometry();
  QPoint CalculateWidgetPosition() const;

  QWidget* m_overlay_widget{nullptr};
  QWidget* m_area{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_
