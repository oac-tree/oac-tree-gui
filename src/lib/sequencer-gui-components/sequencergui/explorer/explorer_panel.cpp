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

#include "explorer_panel.h"

#include <sequencergui/components/procedure_list_widget.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/widgets/file_tree_view.h>
#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QSettings>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

namespace
{
const QString kGroupName = "ExplorerPanel";
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
}  // namespace

namespace sequencergui
{
ExplorerPanel::ExplorerPanel(QWidget *parent)
    : QWidget(parent)
    , m_collapsible_list(new sup::gui::CollapsibleListView)
    , m_file_tree_view(new FileTreeView)
    , m_procedure_list_view(new ProcedureListWidget)
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  m_collapsible_list->setWindowTitle("Explorer View");

  m_collapsible_list->AddWidget(m_file_tree_view);
  m_procedure_list_view->SetupActions(ProcedureListWidget::kRemoveSelected);
  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view, m_procedure_list_view->actions());

  m_stack_widget->AddWidget(m_collapsible_list, m_file_tree_view->actions());

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_stack_widget);

  connect(m_file_tree_view, &FileTreeView::ProcedureFileClicked, this,
          &ExplorerPanel::ProcedureFileClicked);
  connect(m_file_tree_view, &FileTreeView::ProcedureFileDoubleClicked, this,
          &ExplorerPanel::ProcedureFileDoubleClicked);
  connect(m_procedure_list_view, &ProcedureListWidget::RemoveProcedureRequest, this,
          &ExplorerPanel::RemoveProcedureRequest);

  ReadSettings();
}

ExplorerPanel::~ExplorerPanel()
{
  WriteSettings();
}

void ExplorerPanel::SetModel(SequencerModel *model)
{
  m_procedure_list_view->SetModel(model);
}

ProcedureItem *ExplorerPanel::GetSelectedProcedure()
{
  return m_procedure_list_view->GetSelectedProcedure();
}

void ExplorerPanel::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_collapsible_list->GetSplitter()->restoreState(
        settings.value(kSplitterSettingName).toByteArray());
  }
}

void ExplorerPanel::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_collapsible_list->GetSplitter()->saveState());
}

}  // namespace sequencergui
