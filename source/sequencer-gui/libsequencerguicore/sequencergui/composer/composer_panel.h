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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
#define SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class InstructionItemPanel;
class ProcedureListWidget;
class PanelToolBar;
class SequencerModel;
class ProcedureItem;
class AggregatePanel;
class CollapsibleListView;

//! Left panel on SequencerComposerView.
//! Contains list of opened procedures, list of available instruction, and list of available
//! variables.

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
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest(sequencergui::ProcedureItem* procedure_item);

private:
  PanelToolBar* m_tool_bar{nullptr};
  CollapsibleListView* m_collapsible_list{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
  InstructionItemPanel* m_instruction_panel{nullptr};
  AggregatePanel* m_workspace_panel{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
