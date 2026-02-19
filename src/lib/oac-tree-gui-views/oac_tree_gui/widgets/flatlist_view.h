/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_WIDGETS_FLATLIST_VIEW_H_
#define OAC_TREE_GUI_WIDGETS_FLATLIST_VIEW_H_

#include <QTreeView>

namespace oac_tree_gui
{

class FlatListDelegate;

/**
 * @brief The FlatListView class shows flat list of items and provides machinery to remove items
 * from the list.
 *
 * It is a two-column tree tuned to look like a list, with close indicator in a second column.
 * Intended to work together with FlatListDelegate.
 *
 * Copied from QtCreator's opendocumentstreeview.cpp, OpenDocumentsTreeView class.
 */
class FlatListView : public QTreeView
{
  Q_OBJECT

public:
  explicit FlatListView(QWidget* parent = nullptr);

  void setModel(QAbstractItemModel* model) override;
  void setCloseButtonVisible(bool visible);

  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;

signals:
  void closeActivated(const QModelIndex& index);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  FlatListDelegate* m_delegate{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_WIDGETS_FLATLIST_VIEW_H_
