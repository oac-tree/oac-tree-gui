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

#include <oac_tree_gui/views/nodeeditor/zoom_factor_converter.h>

class QSlider;
class QLabel;

namespace oac_tree_gui
{

class NodeEditorNavigationToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit NodeEditorNavigationToolBar(QWidget* parent_widget = nullptr);

  void SetZoomFactor(double zoom_factor);

signals:
  void ZoomFactorRequest(double zoom_factor);

private:
  void InsertStrech();

  QSlider* m_zoom_slider{nullptr};
  QLabel* m_zoom_label{nullptr};
  bool m_is_interactive{false};
  ZoomFactorConverter m_zoom_factor_converter;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_NODE_EDITOR_NAVIGATION_TOOLBAR_H_
