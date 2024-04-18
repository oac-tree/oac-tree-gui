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

#include "procedure_list_widget.h"

#include "procedure_list_action_handler.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/viewmodel/procedure_viewmodel.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QListView>
#include <QMenu>
#include <QMimeData>
#include <QVBoxLayout>

namespace sequencergui
{

ProcedureListWidget::ProcedureListWidget(QWidget *parent)
    : QWidget(parent)
    , m_list_view(new QListView)
    , m_component_provider(mvvm::CreateProvider<ProcedureViewModel>(m_list_view))
    , m_actions(new ProcedureListActions(this))
    , m_action_handler(new ProcedureListActionHandler(CreateContext(), this))
{
  setWindowTitle("PROCEDURES");
  setToolTip("List of currently opened procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit ProcedureSelected(GetSelectedProcedure()); });

  connect(m_actions, &ProcedureListActions::CreateNewProcedureRequest, m_action_handler,
          &ProcedureListActionHandler::OnCreateNewProcedureRequest);
  connect(m_actions, &ProcedureListActions::RemoveProcedureRequest, m_action_handler,
          &ProcedureListActionHandler::OnRemoveProcedureRequest);
  connect(m_action_handler, &ProcedureListActionHandler::SelectProcedureRequest, this,
          &ProcedureListWidget::SetSelectedProcedure);

  connect(m_actions, &ProcedureListActions::CutRequest, m_action_handler,
          &ProcedureListActionHandler::Cut);
  connect(m_actions, &ProcedureListActions::CopyRequest, m_action_handler,
          &ProcedureListActionHandler::Copy);
  connect(m_actions, &ProcedureListActions::PasteRequest, m_action_handler,
          &ProcedureListActionHandler::Paste);

  m_list_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_list_view, &QListView::customContextMenuRequested, this,
          &ProcedureListWidget::OnContextMenuRequest);
}

ProcedureListWidget::~ProcedureListWidget() = default;

void ProcedureListWidget::SetModel(SequencerModel *model)
{
  m_model = model;
  if (model)
  {
    m_component_provider->SetItem(model->GetProcedureContainer());
    m_listener = std::make_unique<listener_t>(model);

    m_listener->Connect<mvvm::ModelResetEvent>(this, &ProcedureListWidget::OnProjectLoad);
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

QList<QAction *> ProcedureListWidget::GetActions(
    const std::vector<ProcedureListActions::ActionKey> &action_keys)
{
  return m_actions->GetActions(action_keys);
}

void ProcedureListWidget::keyPressEvent(QKeyEvent *event)
{
  if (event == QKeySequence::Cut)
  {
    m_action_handler->Cut();
  }
  else if (event == QKeySequence::Copy)
  {
    m_action_handler->Copy();
  }
  else if (event == QKeySequence::Paste)
  {
    m_action_handler->Paste();
  }
  else
  {
    QWidget::keyPressEvent(event);
  }
}

void ProcedureListWidget::OnProjectLoad(const mvvm::ModelResetEvent &event)
{
  // (void)event;
  auto procedures = m_model->GetProcedures();
  if (!procedures.empty())
  {
    SetSelectedProcedure(procedures.at(0));
  }
}

ProcedureListContext ProcedureListWidget::CreateContext()
{
  auto get_container_callback = [this]()
  { return m_model ? m_model->GetProcedureContainer() : nullptr; };
  auto get_selected_procedure_callback = [this]() { return GetSelectedProcedure(); };

  auto get_mime_data_callback = []() { return QGuiApplication::clipboard()->mimeData(); };

  auto set_mime_data_callback = [](std::unique_ptr<QMimeData> data)
  { return QGuiApplication::clipboard()->setMimeData(data.release()); };

  return {get_container_callback, get_selected_procedure_callback, get_mime_data_callback,
          set_mime_data_callback};
}

void ProcedureListWidget::OnContextMenuRequest(const QPoint &point)
{
  QMenu menu;
  m_actions->SetupMenu(menu, m_action_handler);
  menu.exec(m_list_view->mapToGlobal(point));
}

}  // namespace sequencergui
