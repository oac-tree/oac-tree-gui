/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_EXPLORER_PROCEDURE_TREES_WIDGET_H_
#define SEQUENCERGUI_VIEWS_EXPLORER_PROCEDURE_TREES_WIDGET_H_

#include <QWidget>

class QSplitter;
class QTreeView;

namespace mvvm
{
class PropertyTreeView;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
class ItemStackWidget;
}  // namespace sup::gui

namespace sequencergui
{
class SequencerModel;
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

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};

  QTreeView* m_procedure_tree{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_procedure_tree_provider;
  sup::gui::CustomHeaderView* m_procedure_custom_header{nullptr};

  mvvm::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_EXPLORER_PROCEDURE_TREES_WIDGET_H_
