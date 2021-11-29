/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H

#include <QWidget>

namespace ModelView
{
class TopItemsTreeView;
class PropertyTreeView;
}  // namespace ModelView

namespace sequi
{
class SequencerModel;
class ExplorerToolBar;
class ProcedureItem;

//! The panel with two trees in the middle of SequencerXMLView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! procedure's element.

class ProcedureTreesWidget : public QWidget
{
  Q_OBJECT

public:
  ProcedureTreesWidget(QWidget* parent = nullptr);

  void SetModel(SequencerModel* model, ProcedureItem* procedure_item = nullptr);

private:
  ExplorerToolBar* m_tool_bar{nullptr};
  ModelView::TopItemsTreeView* m_procedure_tree{nullptr};
  ModelView::PropertyTreeView* m_property_tree{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H
