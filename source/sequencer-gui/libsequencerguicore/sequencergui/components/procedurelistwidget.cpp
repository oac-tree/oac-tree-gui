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

#include "mvvm/model/itemutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/viewmodel/viewmodelutils.h"
#include "mvvm/widgets/abstractitemview.h"

#include <QAction>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>

namespace sequencergui
{

class ProcedureList : public mvvm::AbstractItemView
{
public:
  explicit ProcedureList(mvvm::ApplicationModel *model = nullptr, QWidget *parent = nullptr)
      : AbstractItemView(mvvm::CreateViewModel<mvvm::TopItemsViewModel>, new QListView, model,
                         parent)
  {
  }
};

ProcedureListWidget::ProcedureListWidget(QWidget *parent)
    : QWidget(parent)
    , m_new_procedure_action(new QAction)
    , m_remove_selected_action(new QAction)
    , m_list_view(new ProcedureList)
{
  setWindowTitle("PROCEDURES");
  setToolTip("List of currently opened procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);
}

ProcedureListWidget::~ProcedureListWidget() = default;

void ProcedureListWidget::SetModel(SequencerModel *model)
{
  m_model = model;
  if (model)
  {
    m_list_view->SetItem(model->GetProcedureContainer());
  }

  connect(m_list_view, &ProcedureList::SelectedItemChanged, this,
          [this](auto) { emit ProcedureSelected(GetSelectedProcedure()); });
}

ProcedureItem *ProcedureListWidget::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<ProcedureItem *> ProcedureListWidget::GetSelectedProcedures() const
{
  std::vector<ProcedureItem *> result;
  auto selected = m_list_view->GetSelectedItems();
  auto on_item = [](auto it)
  { return dynamic_cast<ProcedureItem *>(const_cast<mvvm::SessionItem *>(it)); };
  std::transform(selected.begin(), selected.end(), std::back_inserter(result), on_item);
  return result;
}

void ProcedureListWidget::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_list_view->SetSelectedItem(procedure);
}

QListView *ProcedureListWidget::GetListView()
{
  return dynamic_cast<QListView *>(m_list_view->GetView());
}

mvvm::ViewModel *ProcedureListWidget::GetViewModel()
{
  return m_list_view->GetViewModel();
}

void ProcedureListWidget::SetupActions(int action_flag)
{
  if (action_flag & kCreateNew)
  {
    m_new_procedure_action->setText("New");
    m_new_procedure_action->setToolTip("Create new empty procedure");
    m_new_procedure_action->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
    connect(m_new_procedure_action, &QAction::triggered, this,
            &ProcedureListWidget::CreateNewProcedureRequest);
    addAction(m_new_procedure_action);
  }

  if (action_flag & kRemoveSelected)
  {
    m_remove_selected_action->setText("Remove");
    m_remove_selected_action->setToolTip("Remove selected procedure");
    m_remove_selected_action->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
    auto on_remove = [this]() { emit RemoveProcedureRequest(GetSelectedProcedure()); };
    connect(m_remove_selected_action, &QAction::triggered, this, on_remove);
    addAction(m_remove_selected_action);
  }
}

}  // namespace sequencergui
