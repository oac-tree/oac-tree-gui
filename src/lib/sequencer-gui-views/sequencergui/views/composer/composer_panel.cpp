/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "composer_panel.h"

#include "aggregate_panel.h"
#include "instruction_item_panel.h"
#include "procedure_list_widget.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/container_item.h>

#include <QVBoxLayout>

namespace sequencergui
{

ComposerPanel::ComposerPanel(QWidget *parent)
    : QWidget(parent)
    , m_collapsible_list(new sup::gui::CollapsibleListView)
    , m_procedure_list_view(new ProcedureListWidget)
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
  m_collapsible_list->AddCollapsibleWidget(m_aggregate_panel, {});

  auto toolbar_actions =
      m_procedure_list_view->GetActions({ProcedureListActions::ActionKey::kCreateNew,
                                         ProcedureListActions::ActionKey::kRemoveSelected});
  m_stack_widget->AddWidget(m_collapsible_list, toolbar_actions);

  connect(m_procedure_list_view, &ProcedureListWidget::ProcedureSelected, this,
          &ComposerPanel::ProcedureSelected);

  auto append_instruction = [this](const QString &name)
  {
    if (auto procedure = GetSelectedProcedure(); procedure)
    {
      InsertInstruction(name.toStdString(), procedure->GetInstructionContainer(),
                        mvvm::TagIndex::Append());
    }
  };
  connect(m_instruction_panel, &InstructionItemPanel::InstructionDoubleClicked, this,
          append_instruction);
}

ComposerPanel::~ComposerPanel() = default;

void ComposerPanel::SetModel(SequencerModel *model)
{
  m_procedure_list_view->SetModel(model);

  auto procedure_container = model ? model->GetProcedureContainer() : nullptr;
  if (procedure_container)
  {
    SetSelectedProcedure(procedure_container->GetItem<ProcedureItem>(mvvm::TagIndex::First()));
  }
}

ProcedureItem *ComposerPanel::GetSelectedProcedure()
{
  return m_procedure_list_view->GetSelectedProcedure();
}

void ComposerPanel::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_procedure_list_view->SetSelectedProcedure(procedure);
}

}  // namespace sequencergui
