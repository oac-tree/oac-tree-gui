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

#include "realtime_tree_view.h"

#include <QDebug>
#include <QEvent>
#include <QFontMetrics>
#include <QHeaderView>
#include <QPainter>
#include <QScrollBar>

namespace sequencergui
{

ExperimentalBreakpointTreeView::ExperimentalBreakpointTreeView(QWidget *parent) : QTreeView(parent)
{
  m_left_margin = fontMetrics().height();

  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ExperimentalBreakpointTreeView::updateLeftMargin);
  connect(this, &ExperimentalBreakpointTreeView::expanded, this, &ExperimentalBreakpointTreeView::updateLeftMargin);
  connect(this, &ExperimentalBreakpointTreeView::collapsed, this, &ExperimentalBreakpointTreeView::updateLeftMargin);
}

void ExperimentalBreakpointTreeView::setModel(QAbstractItemModel *new_model)
{
  if (model() == new_model)
  {
    return;
  }

  if (model())
  {
    disconnect(model(), &QAbstractItemModel::dataChanged, this,
               &ExperimentalBreakpointTreeView::updateLeftMargin);
  }

  QTreeView::setModel(new_model);

  if (model())
  {
    connect(model(), &QAbstractItemModel::dataChanged, this, &ExperimentalBreakpointTreeView::updateLeftMargin);
  }
}

void ExperimentalBreakpointTreeView::updateGeometries()
{
  QTreeView::updateGeometries();
  auto margins = viewportMargins();
  if (margins.left() < m_left_margin)
  {
    margins.setLeft(margins.left() + m_left_margin);
    setViewportMargins(margins);
  }
}

void ExperimentalBreakpointTreeView::paintEvent(QPaintEvent *event)
{
  QTreeView::paintEvent(event);

  auto pos = QPoint();
  auto index = indexAt(pos);
  QPainter qp(viewport());
  auto border = frameWidth();
  auto bottom = height() - border * 2;
  qp.setClipRect(QRect(border, border, m_left_margin, bottom));
  double top = 0.5;
  if (header()->isVisible())
  {
    top += header()->height();
  }
  qp.translate(0.5, top);
  qp.setBrush(Qt::red);
  qp.setPen(Qt::red);
  qp.setRenderHints(QPainter::Antialiasing);
  int deltaY = m_left_margin / 2 - border;
  auto circle = QRect(border + 1, 0, m_left_margin - 2, m_left_margin - 2);
  int row = 0;

  while (index.isValid())
  {
    qDebug() << "1.1";
    auto rect = visualRect(index);
    if (true)  // breakpoint role
    {
      qDebug() << "1.2";
      circle.moveTop(rect.center().y() - deltaY);
      qp.drawEllipse(circle);
    }
    row += 1;
    pos.setY(rect.bottom() + 2);
    if (pos.y() > bottom)
    {
      break;
    }
    index = indexAt(pos);
  }
}

void ExperimentalBreakpointTreeView::updateLeftMargin()
{
  QWidget::update(QRect(0, 0, m_left_margin + frameWidth(), height()));
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

QStyleOptionViewItem ExperimentalBreakpointTreeView::viewOptions() const
{
  auto options = QTreeView::viewOptions();
  options.decorationAlignment = Qt::AlignHCenter | Qt::AlignCenter;
  options.decorationPosition = QStyleOptionViewItem::Top;
  return options;
}

#else
void RealTimeTreeView::initViewItemOption(QStyleOptionViewItem *option) const
{
  options->decorationAlignment = Qt::AlignHCenter | Qt::AlignCenter;
  options->decorationPosition = QStyleOptionViewItem::Top;
}
#endif

}  // namespace sequencergui
