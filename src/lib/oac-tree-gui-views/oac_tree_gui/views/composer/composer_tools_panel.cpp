/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "composer_tools_panel.h"

#include "aggregate_panel.h"
#include "instruction_item_panel.h"
#include "procedure_list_widget.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/container_item.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{
const QString& kCollapsibleListSettingName = "ComposerPanel/collapsible_list";
}

ComposerToolsPanel::ComposerToolsPanel(sup::gui::IAppCommandService& command_service,
                                       QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_collapsible_list(new sup::gui::CollapsibleListView(kCollapsibleListSettingName))
    , m_procedure_list_view(new ProcedureListWidget(command_service))
    , m_instruction_panel(new InstructionItemPanel)
    , m_aggregate_panel(new AggregatePanel)
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  m_collapsible_list->setWindowTitle("Procedures & Tools");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_stack_widget);

  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view, {});
  m_collapsible_list->AddCollapsibleWidget(m_instruction_panel, {});
  m_collapsible_list->AddCollapsibleWidget(m_aggregate_panel, {}, false);

  auto toolbar_actions =
      m_procedure_list_view->GetActions({ProcedureListActions::ActionKey::kCreateNew,
                                         ProcedureListActions::ActionKey::kRemoveSelected});
  m_stack_widget->AddWidget(m_collapsible_list, toolbar_actions);

  connect(m_procedure_list_view, &ProcedureListWidget::ProcedureSelected, this,
          &ComposerToolsPanel::ProcedureSelected);

  connect(m_instruction_panel, &InstructionItemPanel::InstructionDoubleClicked, this,
          &ComposerToolsPanel::ToolBoxInstructionRequest);
  connect(m_aggregate_panel, &AggregatePanel::InstructionDoubleClicked, this,
          &ComposerToolsPanel::ToolBoxInstructionRequest);

  ReadSettings();
}

ComposerToolsPanel::~ComposerToolsPanel()
{
  WriteSettings();
}

void ComposerToolsPanel::SetModel(SequencerModel* model)
{
  m_procedure_list_view->SetModel(model);

  auto procedure_container = (model != nullptr) ? model->GetProcedureContainer() : nullptr;
  if (procedure_container != nullptr)
  {
    SetSelectedProcedure(procedure_container->GetItem<ProcedureItem>(mvvm::TagIndex::First()));
  }
}

ProcedureItem* ComposerToolsPanel::GetSelectedProcedure()
{
  return m_procedure_list_view->GetSelectedProcedure();
}

void ComposerToolsPanel::SetSelectedProcedure(ProcedureItem* procedure)
{
  m_procedure_list_view->SetSelectedProcedure(procedure);
}

void ComposerToolsPanel::ReadSettings()
{
  m_collapsible_list->ReadSettings();
}

void ComposerToolsPanel::WriteSettings()
{
  m_collapsible_list->WriteSettings();
}

}  // namespace oac_tree_gui
