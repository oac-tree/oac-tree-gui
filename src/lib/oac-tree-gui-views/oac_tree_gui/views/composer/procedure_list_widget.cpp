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

#include "procedure_list_widget.h"

#include <oac_tree_gui/composer/objects/procedure_list_action_handler.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/mainwindow/clipboard_helper.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

namespace oac_tree_gui
{

ProcedureListWidget::ProcedureListWidget(sup::gui::IAppCommandService& command_service,
                                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_list_view(new QListView)
    , m_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
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

  auto context = m_command_service.RegisterWidgetUniqueId(this);
  m_actions->RegisterActionsForContext(context, m_command_service);
}

ProcedureListWidget::~ProcedureListWidget()
{
  m_command_service.UnregisterWidgetUniqueId(this);
}

void ProcedureListWidget::SetModel(SequencerModel* model)
{
  m_model = model;
  if (model != nullptr)
  {
    m_component_provider->SetItem(model->GetProcedureContainer());
  }
}

ProcedureItem* ProcedureListWidget::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<ProcedureItem*> ProcedureListWidget::GetSelectedProcedures() const
{
  return m_component_provider->GetSelectedItems<ProcedureItem>();
}

void ProcedureListWidget::SetSelectedProcedure(ProcedureItem* procedure)
{
  m_component_provider->SetSelectedItem(procedure);
}

QListView* ProcedureListWidget::GetListView()
{
  return m_list_view;
}

mvvm::ViewModel* ProcedureListWidget::GetViewModel()
{
  return m_component_provider->GetViewModel();
}

QList<QAction*> ProcedureListWidget::GetActions(
    const std::vector<ProcedureListActions::ActionKey>& action_keys)
{
  return m_actions->GetActions(action_keys);
}

ProcedureListContext ProcedureListWidget::CreateContext()
{
  ProcedureListContext result;
  result.procedure_container = [this]()
  { return m_model ? m_model->GetProcedureContainer() : nullptr; };
  result.selected_procedure = [this]() { return GetSelectedProcedure(); };
  result.get_mime_data = sup::gui::DefaultClipboardGetFunc();
  result.set_mime_data = sup::gui::DefaultClipboardSetFunc();
  return result;
}

void ProcedureListWidget::OnContextMenuRequest(const QPoint& point)
{
  QMenu menu;
  m_actions->SetupMenu(menu, m_action_handler);
  menu.exec(m_list_view->mapToGlobal(point));
}

}  // namespace oac_tree_gui
