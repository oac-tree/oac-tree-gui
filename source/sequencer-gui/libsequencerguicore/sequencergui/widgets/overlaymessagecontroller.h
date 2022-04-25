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

#ifndef SEQUENCERGUI_WIDGETS_OVERLAYMESSAGECONTROLLER_H
#define SEQUENCERGUI_WIDGETS_OVERLAYMESSAGECONTROLLER_H

#include <QFrame>

class QAbstractScrollArea;
class QPoint;

namespace sequencergui
{

class OverlayMessageFrame;

//! Controls appearance of OverlayMessageFrame (position, show/hide) on top of widget.
//! Current implementation places message box at the lower bottom conrner of widget (hardcoded
//! behavior for the moment).

class OverlayMessageController : public QObject
{
  Q_OBJECT

public:
  explicit OverlayMessageController(OverlayMessageFrame* message, QAbstractScrollArea* area,
                                    QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  void UpdateLabelGeometry();
  QPoint GetBoxPosition() const;

  OverlayMessageFrame* m_message_frame{nullptr};
  QAbstractScrollArea* m_area{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_OVERLAYMESSAGECONTROLLER_H
