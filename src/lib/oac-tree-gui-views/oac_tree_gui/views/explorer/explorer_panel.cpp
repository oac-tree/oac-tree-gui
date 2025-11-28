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

#include "explorer_panel.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/views/composer/procedure_list_widget.h>
#include <oac_tree_gui/widgets/file_tree_view.h>

#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QToolButton>
#include <QVBoxLayout>

namespace
{
const QString kCollapsibleListSettingName = "ExplorerPanel/collapsible_list";
}  // namespace

namespace oac_tree_gui
{
ExplorerPanel::ExplorerPanel(sup::gui::IAppCommandService& command_service, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_collapsible_list(new sup::gui::CollapsibleListView(kCollapsibleListSettingName))
    , m_file_tree_view(new FileTreeView)
    , m_procedure_list_view(new ProcedureListWidget(command_service))
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  m_collapsible_list->setWindowTitle("Explorer View");

  m_collapsible_list->AddWidget(m_file_tree_view);

  auto toolbar_actions =
      m_procedure_list_view->GetActions({ProcedureListActions::ActionKey::kRemoveSelected});
  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view, toolbar_actions);

  m_stack_widget->AddWidget(m_collapsible_list, m_file_tree_view->actions());

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_stack_widget);

  connect(m_file_tree_view, &FileTreeView::FileTreeClicked, this, &ExplorerPanel::FileTreeClicked);
  connect(m_file_tree_view, &FileTreeView::ProcedureFileDoubleClicked, this,
          &ExplorerPanel::ProcedureFileDoubleClicked);

  ReadSettings();
}

ExplorerPanel::~ExplorerPanel()
{
  WriteSettings();
}

void ExplorerPanel::SetModel(SequencerModel* model)
{
  m_procedure_list_view->SetModel(model);
}

void ExplorerPanel::ReadSettings()
{
  m_collapsible_list->ReadSettings();
}

void ExplorerPanel::WriteSettings()
{
  m_collapsible_list->WriteSettings();
}

}  // namespace oac_tree_gui
