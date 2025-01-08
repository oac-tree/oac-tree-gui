/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "overlay_message_frame.h"

#include <mvvm/widgets/widget_utils.h>

#include <QColor>
#include <QFont>
#include <QPainter>

namespace
{
//! Returns width of message box.
int GetMaximumBoxWidth()
{
  return mvvm::utils::UnitSize(40);
}

//! Returns height of message box.
int GetMinimumBoxHeight()
{
  return mvvm::utils::UnitSize(4);
}

}  // namespace

namespace sequencergui
{

OverlayMessageFrame::OverlayMessageFrame(const QString& text, QWidget* parent_widget)
    : QFrame(parent_widget)
    , m_text(text)
    , m_bounding_rect(0, 0, GetMaximumBoxWidth(), GetMinimumBoxHeight())
    , m_font("Monospace", mvvm::utils::SystemPointSize(), QFont::Normal, true)
{
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_NoSystemBackground);
  setGeometry(0, 0, m_bounding_rect.width(), m_bounding_rect.height());
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
  painter.setFont(m_font);
  painter.drawRect(m_bounding_rect);
  auto margin = mvvm::utils::UnitSize();
  painter.drawText(m_bounding_rect.marginsRemoved(QMargins(margin, margin, margin, margin)),
                   Qt::TextWordWrap, m_text);
}

}  // namespace sequencergui
