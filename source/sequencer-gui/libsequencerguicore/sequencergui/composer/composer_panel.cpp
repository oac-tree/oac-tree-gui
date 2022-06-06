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

#include "sequencergui/composer/composer_panel.h"

#include "sequencergui/components/procedure_list_widget.h"
#include "sequencergui/composer/aggregate_panel.h"
#include "sequencergui/composer/instruction_item_panel.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/widgets/collapsible_list_view.h"
#include "sequencergui/widgets/item_list_widget.h"
#include "sequencergui/widgets/panel_toolbar.h"

#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

ComposerPanel::ComposerPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_collapsible_list(new CollapsibleListView)
    , m_procedure_list_view(new ProcedureListWidget)
    , m_instruction_panel(new InstructionItemPanel)
    , m_workspace_panel(new AggregatePanel)

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_collapsible_list);

  m_procedure_list_view->SetupActions(ProcedureListWidget::kCreateNew
                                      | ProcedureListWidget::kRemoveSelected);
  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view, m_procedure_list_view->actions());
  m_collapsible_list->AddCollapsibleWidget(m_instruction_panel, {});
  m_collapsible_list->AddCollapsibleWidget(m_workspace_panel, {});

  connect(m_procedure_list_view, &ProcedureListWidget::ProcedureSelected, this,
          &ComposerPanel::ProcedureSelected);
  connect(m_procedure_list_view, &ProcedureListWidget::CreateNewProcedureRequest, this,
          &ComposerPanel::CreateNewProcedureRequest);
  connect(m_procedure_list_view, &ProcedureListWidget::RemoveProcedureRequest, this,
          &ComposerPanel::RemoveProcedureRequest);
}

ComposerPanel::~ComposerPanel() = default;

void ComposerPanel::SetModel(SequencerModel *model)
{
  m_procedure_list_view->SetModel(model);
}

ProcedureItem *ComposerPanel::GetSelectedProcedure()
{
  return m_procedure_list_view->GetSelectedProcedure();
}

void ComposerPanel::SetSelectedProcedure(ProcedureItem *procedure)
{
  return m_procedure_list_view->SetSelectedProcedure(procedure);
}

}  // namespace sequencergui
