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

namespace sequencergui
{

RealTimeTreeView::RealTimeTreeView(QWidget *parent) : QTreeView(parent) {}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

QStyleOptionViewItem RealTimeTreeView::viewOptions() const
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
