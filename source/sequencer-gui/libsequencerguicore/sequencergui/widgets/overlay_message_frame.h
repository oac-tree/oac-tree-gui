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

#ifndef SEQUENCERGUI_WIDGETS_OVERLAYMESSAGEFRAME_H
#define SEQUENCERGUI_WIDGETS_OVERLAYMESSAGEFRAME_H

#include <QFrame>
#include <QFont>

namespace sequencergui
{

//! The overlay frame with the text message to place on top of any widget outside of any layout
//! context.

class OverlayMessageFrame : public QFrame
{
  Q_OBJECT

public:
  explicit OverlayMessageFrame(const QString& text, QWidget* parent = nullptr);

//  void SetRectangle(const QRect& rect);
  void SetPosition(int x, int y);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QString m_text;
  QRect m_bounding_rect;
  QFont m_font;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_OVERLAYMESSAGEFRAME_H
