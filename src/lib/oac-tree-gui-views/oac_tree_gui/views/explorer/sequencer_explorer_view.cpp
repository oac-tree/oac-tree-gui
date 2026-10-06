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

#include "sequencer_explorer_view.h"

#include "explorer_panel.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/views/operation/procedure_action_handler.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/bin_utils.h>

#include <QVBoxLayout>

namespace
{

//! Settings group of the main splitter.
constexpr auto kSplitterSettingsGroup = "SequencerExplorerView/splitter";

}  // namespace

namespace oac_tree_gui
{

SequencerExplorerView::SequencerExplorerView(const sup::gui::AppViewContext& context,
                                             QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_explorer_panel(new ExplorerPanel(context))
    , m_xml_view(new sup::gui::CodeView(sup::gui::CodeView::kXML, context.settings_manager))
    , m_right_panel(new sup::gui::ItemStackWidget)
    , m_splitter(new sup::gui::SettingsSplitter(kSplitterSettingsGroup, context.settings_manager))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_explorer_panel);

  m_right_panel->AddWidget(m_xml_view);
  m_splitter->addWidget(m_right_panel);

  layout->addWidget(m_splitter);

  SetupConnections();

  m_splitter->RestoreState();
}

SequencerExplorerView::~SequencerExplorerView() = default;

//! Sets the model.
void SequencerExplorerView::SetModel(SequencerModel* model)
{
  m_model = model;
  m_explorer_panel->SetModel(model);
}

void SequencerExplorerView::ImportProcedure(const QString& file_name)
{
  ProcedureActionHandler handler;

  if (auto procedure_item = handler.LoadProcedureFromFile(file_name); procedure_item)
  {
    m_model->InsertItem(std::move(procedure_item), m_model->GetProcedureContainer(),
                        mvvm::TagIndex::Append());
  }
}

void SequencerExplorerView::ShowFileContent(const QString& file_name)
{
  if (mvvm::utils::IsBinaryFile(file_name.toStdString()))
  {
    m_xml_view->ClearText();
  }
  else
  {
    m_xml_view->SetFile(file_name);
  }
}

void SequencerExplorerView::SetupConnections()
{
  connect(m_explorer_panel, &ExplorerPanel::FileTreeClicked, this,
          &SequencerExplorerView::ShowFileContent);

  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileDoubleClicked, this,
          &SequencerExplorerView::ImportProcedure);
}

}  // namespace oac_tree_gui
