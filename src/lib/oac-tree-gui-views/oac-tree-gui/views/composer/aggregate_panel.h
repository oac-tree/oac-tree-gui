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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_AGGREGATE_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_AGGREGATE_PANEL_H_

#include <QWidget>

namespace oac_tree_gui
{

class ToolKitViewModel;
class InsructionToolKitTreeView;

//! Panel with tree view for instruction aggregates.

class AggregatePanel : public QWidget
{
  Q_OBJECT

public:
  explicit AggregatePanel(QWidget* parent_widget = nullptr);

private:
  ToolKitViewModel* m_toolkit_viewmodel{nullptr};
  InsructionToolKitTreeView* m_tree_view{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_AGGREGATE_PANEL_H_
