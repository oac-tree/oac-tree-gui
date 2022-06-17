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

#include "sequencergui/widgets/custom_header_view.h"

#include <QDebug>

namespace sequencergui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : QHeaderView(Qt::Horizontal, parent) {}

void CustomHeaderView::RestoreSize() {}

void CustomHeaderView::mousePressEvent(QMouseEvent *event)
{
  m_is_in_interactive_mode = true;
  qDebug() << "pressed";
  QHeaderView::mousePressEvent(event);
}

void CustomHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
  m_is_in_interactive_mode = false;
  qDebug() << "released";
  QHeaderView::mouseReleaseEvent(event);
}

void CustomHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  if (!m_is_in_interactive_mode)
  {
    return;
  }
}

}  // namespace sequencergui
