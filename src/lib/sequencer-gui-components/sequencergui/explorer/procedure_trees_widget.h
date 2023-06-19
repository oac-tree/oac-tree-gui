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

#ifndef SEQUENCERGUI_EXPLORER_PROCEDURE_TREES_WIDGET_H_
#define SEQUENCERGUI_EXPLORER_PROCEDURE_TREES_WIDGET_H_

#include <QWidget>

class QSplitter;

namespace mvvm
{
class TopItemsTreeView;
class PropertyTreeView;
}  // namespace mvvm

namespace sequencergui
{
class SequencerModel;
class PanelToolBar;
class ProcedureItem;

//! The panel with two trees in the middle of SequencerXMLView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! procedure's element.

class ProcedureTreesWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureTreesWidget(QWidget* parent = nullptr);
  ~ProcedureTreesWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

private:
  void ReadSettings();
  void WriteSettings();

  PanelToolBar* m_tool_bar{nullptr};
  mvvm::TopItemsTreeView* m_procedure_tree{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_PROCEDURE_TREES_WIDGET_H_
