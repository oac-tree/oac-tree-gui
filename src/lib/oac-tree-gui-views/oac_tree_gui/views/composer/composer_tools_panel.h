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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_TOOLS_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_TOOLS_PANEL_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{
class CollapsibleListView;
class ItemStackWidget;
}  // namespace sup::gui

namespace oac_tree_gui
{

class InstructionItemPanel;
class ProcedureListWidget;
class SequencerModel;
class ProcedureItem;
class AggregatePanel;

/**
 * @brief The ComposerToolsPanel class represent a left panel of SequencerComposerView (second main
 * view).
 *
 * Contains list of opened procedures, lists of available instructions and aggregates.
 */
class ComposerToolsPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerToolsPanel(QWidget* parent_widget = nullptr);
  ~ComposerToolsPanel() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void ProcedureSelected(oac_tree_gui::ProcedureItem* procedureItem);

  /**
   * @brief Emmited when the user double clicks on instruction/aggregate name in the left
   * instruction tool box.
   */
  void ToolBoxInstructionRequest(const QString& name);

private:
  void ReadSettings();
  void WriteSettings();

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
  InstructionItemPanel* m_instruction_panel{nullptr};
  AggregatePanel* m_aggregate_panel{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_TOOLS_PANEL_H_
