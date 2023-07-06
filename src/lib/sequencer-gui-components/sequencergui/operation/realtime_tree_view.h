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

#ifndef SEQUENCERGUI_OPERATION_REALTIME_TREE_VIEW_H_
#define SEQUENCERGUI_OPERATION_REALTIME_TREE_VIEW_H_

#include <QTreeView>

namespace sequencergui
{

/**
 * @brief The RealTimeTreeView class is a QTreeView which is styled to show breakpoints.
 */

class RealTimeTreeView : public QTreeView
{
  Q_OBJECT
public:
  explicit RealTimeTreeView(QWidget *parent = nullptr);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QStyleOptionViewItem viewOptions() const;
#else
  void initViewItemOption(QStyleOptionViewItem *option) const;
#endif
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_REALTIME_TREE_VIEW_H_
