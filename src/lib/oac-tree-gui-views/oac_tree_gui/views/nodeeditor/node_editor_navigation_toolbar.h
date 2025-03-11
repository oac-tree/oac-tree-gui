/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_NODE_EDITOR_NAVIGATION_TOOLBAR_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_NODE_EDITOR_NAVIGATION_TOOLBAR_H_

#include <QToolBar>

class QSlider;
class QLabel;

namespace oac_tree_gui
{

class NodeEditorNavigationToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit NodeEditorNavigationToolBar(QWidget* parent_widget = nullptr);

  void SetZoomFactor(double value);

signals:
  void ZoomFactorRequest(double value);

private:
  void InsertStrech();

  QSlider* m_zoom_slider{nullptr};
  QLabel* m_zoom_label{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_NODE_EDITOR_NAVIGATION_TOOLBAR_H_
