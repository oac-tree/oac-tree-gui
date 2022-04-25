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

#include "sequencergui/widgets/warningsignwidget.h"

#include "sequencergui/mainwindow/styleutils.h"

#include <QIcon>
#include <QMessageBox>
#include <QPainter>
#include <QRect>

namespace sequencergui
{

WarningSignWidget::WarningSignWidget(const QString &message, QWidget *parent)
    : QWidget(parent), m_header("Houston, we have a problem."), m_message(message)
{
  setAttribute(Qt::WA_NoSystemBackground);
  setToolTip("Click to see details");

  m_pixmap = StyleUtils::GetIcon("dots-horizontal.svg").pixmap(QSize(48, 48));

  setGeometry(0, 0, m_pixmap.width(), m_pixmap.height());
}

void WarningSignWidget::SetHeader(const QString &header)
{
  m_header = header;
}

void WarningSignWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  QRect target(m_pixmap.rect());
  painter.drawPixmap(target, m_pixmap);
}

void WarningSignWidget::mousePressEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  QMessageBox box;
  box.setWindowTitle(m_header);
  box.setInformativeText(m_message);
  box.setStandardButtons(QMessageBox::Ok);
  box.setDefaultButton(QMessageBox::Ok);
  box.exec();
}

}  // namespace sequencergui
