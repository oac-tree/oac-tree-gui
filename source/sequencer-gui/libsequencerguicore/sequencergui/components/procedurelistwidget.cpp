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

#include "sequencergui/components/procedurelistwidget.h"

#include "sequencergui/model/instructioncontaineritem.h"
#include "sequencergui/model/procedureitem.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/utils/styleutils.h"

#include "mvvm/model/item_utils.h"
#include "mvvm/standarditems/container_item.h"
#include "mvvm/viewmodel/top_items_viewmodel.h"
#include "mvvm/widgets/itemviewcomponentprovider.h"

#include <QAction>
#include <QListView>
#include <QVBoxLayout>

namespace sequencergui
{

ProcedureListWidget::ProcedureListWidget(QWidget *parent)
    : QWidget(parent)
    , m_new_procedure_action(new QAction)
    , m_remove_selected_action(new QAction)
    , m_list_view(new QListView)
    , m_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
{
  setWindowTitle("PROCEDURES");
  setToolTip("List of currently opened procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit ProcedureSelected(GetSelectedProcedure()); });
}

ProcedureListWidget::~ProcedureListWidget() = default;

void ProcedureListWidget::SetModel(SequencerModel *model)
{
  m_model = model;
  if (model)
  {
    m_component_provider->SetItem(model->GetProcedureContainer());
  }
}

ProcedureItem *ProcedureListWidget::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<ProcedureItem *> ProcedureListWidget::GetSelectedProcedures() const
{
  return m_component_provider->GetSelectedItems<ProcedureItem>();
}

void ProcedureListWidget::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_component_provider->SetSelectedItem(procedure);
}

QListView *ProcedureListWidget::GetListView()
{
  return m_list_view;
}

mvvm::ViewModel *ProcedureListWidget::GetViewModel()
{
  return m_component_provider->GetViewModel();
}

void ProcedureListWidget::SetupActions(int action_flag)
{
  if (action_flag & kCreateNew)
  {
    m_new_procedure_action->setText("New");
    m_new_procedure_action->setToolTip("Create new empty procedure");
    m_new_procedure_action->setIcon(styleutils::GetIcon("file-plus-outline.svg"));
    connect(m_new_procedure_action, &QAction::triggered, this,
            &ProcedureListWidget::CreateNewProcedureRequest);
    addAction(m_new_procedure_action);
  }

  if (action_flag & kRemoveSelected)
  {
    m_remove_selected_action->setText("Remove");
    m_remove_selected_action->setToolTip("Remove selected procedure");
    m_remove_selected_action->setIcon(styleutils::GetIcon("beaker-remove-outline.svg"));
    auto on_remove = [this]() { emit RemoveProcedureRequest(GetSelectedProcedure()); };
    connect(m_remove_selected_action, &QAction::triggered, this, on_remove);
    addAction(m_remove_selected_action);
  }
}

}  // namespace sequencergui
