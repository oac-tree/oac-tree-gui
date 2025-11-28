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

#include "sequencer_composer_view_v2.h"

#include "composer_tools_panel.h"
#include "procedure_splittable_editor_widget.h"
#include "sequencer_composer_actions.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/widgets/custom_splitter.h>

#include <QAction>
#include <QSplitter>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

namespace
{

constexpr auto kGroupName("SequencerComposerViewV2");

QString GetSplitterSettingKey()
{
  return QString(kGroupName) + "/splitter";
}

}  // namespace

SequencerComposerViewV2::SequencerComposerViewV2(sup::gui::IAppCommandService& command_service,
                                                 QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_composer_actions(new SequencerComposerActions(this))
    , m_splitter(new sup::gui::CustomSplitter(GetSplitterSettingKey()))
    , m_composer_tools_panel(new ComposerToolsPanel(command_service))
    , m_splittable_editor_widget(new ProcedureSplittableEditorWidget(command_service))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_composer_tools_panel);
  m_splitter->addWidget(m_splittable_editor_widget);
  m_splitter->setSizes({200, 400});

  SetupConnections();
  SetupWidgetActions();
  ReadSettings();
}

SequencerComposerViewV2::~SequencerComposerViewV2()
{
  WriteSettings();
}

void SequencerComposerViewV2::SetModel(SequencerModel* model)
{
  m_composer_actions->SetModel(model);
  m_splittable_editor_widget->SetModel(model);
  m_composer_tools_panel->SetModel(model);  // will select first procedure
}

void SequencerComposerViewV2::ReadSettings()
{
  m_splittable_editor_widget->ReadSettings(sup::gui::GetSettingsReadFunc());
  m_splitter->ReadSettings();
}

void SequencerComposerViewV2::WriteSettings()
{
  m_splittable_editor_widget->WriteSettings(sup::gui::GetSettingsWriteFunc());
  m_splitter->WriteSettings();
}

void SequencerComposerViewV2::SetupConnections()
{
  auto on_procedure_selected = [this](ProcedureItem* procedure_item)
  {
    m_splittable_editor_widget->SetProcedure(procedure_item);
    m_composer_actions->SetProcedure(procedure_item);
  };

  connect(m_composer_tools_panel, &ComposerToolsPanel::ProcedureSelected, this,
          on_procedure_selected);

  connect(m_splittable_editor_widget,
          &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged,
          m_composer_tools_panel, &ComposerToolsPanel::SetSelectedProcedure);
}

void SequencerComposerViewV2::SetupWidgetActions()
{
  m_toggle_left_sidebar = new QAction("Show/hide left sidebar", this);
  m_toggle_left_sidebar->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar->setIcon(FindIcon("dock-left"));
  connect(m_toggle_left_sidebar, &QAction::triggered, this, [this](auto)
          { m_composer_tools_panel->setVisible(!m_composer_tools_panel->isVisible()); });

  auto context = m_command_service.RegisterWidgetUniqueId(this);
  m_command_service.AddActionToCommand(m_toggle_left_sidebar,
                                       sup::gui::constants::kToggleLeftPanelCommandId, context);
  m_composer_actions->RegisterActionsForContext(context, m_command_service);
}

}  // namespace oac_tree_gui
