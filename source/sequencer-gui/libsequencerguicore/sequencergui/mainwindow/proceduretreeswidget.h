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

#ifndef SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H

#include <QWidget>

namespace mvvm
{
class TopItemsTreeView;
class PropertyTreeView;
}  // namespace mvvm

namespace sequencergui
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
  mvvm::TopItemsTreeView* m_procedure_tree{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PROCEDURETREESWIDGET_H
