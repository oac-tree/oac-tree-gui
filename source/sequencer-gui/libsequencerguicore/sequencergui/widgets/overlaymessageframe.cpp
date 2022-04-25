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

#include "sequencergui/widgets/overlaymessageframe.h"

#include "mvvm/widgets/widgetutils.h"

#include <QColor>
#include <QFont>
#include <QPainter>

namespace sequencergui
{

OverlayMessageFrame::OverlayMessageFrame(const QString& text, QWidget* parent)
    : QFrame(parent), m_text(text)
{
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_NoSystemBackground);
//  setWindowOpacity(0.2);
}

void OverlayMessageFrame::SetRectangle(const QRect& rect)
{
  m_bounding_rect = rect;
}

void OverlayMessageFrame::SetPosition(int x, int y)
{
  setGeometry(x, y, m_bounding_rect.width(), m_bounding_rect.height());
}

void OverlayMessageFrame::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setBrush(QColor(Qt::lightGray));
  QFont serifFont("Monospace", mvvm::utils::SystemPointSize(), QFont::Normal, true);
  painter.setFont(serifFont);
  painter.drawRect(m_bounding_rect);
  auto margin = mvvm::utils::WidthOfLetterM();
  painter.drawText(m_bounding_rect.marginsRemoved(QMargins(margin, margin, margin, margin)),
                   Qt::AlignCenter, m_text);
}

}  // namespace sequencergui
