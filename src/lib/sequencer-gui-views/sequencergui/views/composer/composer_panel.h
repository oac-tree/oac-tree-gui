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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_PANEL_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_PANEL_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{
class CollapsibleListView;
class ItemStackWidget;
}  // namespace sup::gui

namespace sequencergui
{

class InstructionItemPanel;
class ProcedureListWidget;
class SequencerModel;
class ProcedureItem;
class AggregatePanel;

/**
 * @brief The ComposerPanel class represent a left panel of SequencerComposerView (second main
 * view).
 *
 * Contains list of opened procedures, list of available instruction, and list of available
 * variables.
 */
class ComposerPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerPanel(QWidget* parent = nullptr);
  ~ComposerPanel() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void ProcedureSelected(sequencergui::ProcedureItem* procedureItem);

private:
  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
  InstructionItemPanel* m_instruction_panel{nullptr};
  AggregatePanel* m_aggregate_panel{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_PANEL_H_
