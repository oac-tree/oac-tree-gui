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

#include "sequencer_composer_view.h"

#include "composer_tools_panel.h"
#include "procedure_splittable_editor_widget.h"
#include "sequencer_composer_actions.h"

#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/composer_settings_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/project_model.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/widgets/custom_splitter.h>

#include <mvvm/model/item_selection.h>

#include <QAction>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(const oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

namespace
{

constexpr auto kGroupName("SequencerComposerView");

QString GetSplitterSettingKey()
{
  return QString(kGroupName) + "/splitter";
}

}  // namespace

SequencerComposerView::SequencerComposerView(sup::gui::IAppCommandService& command_service,
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

SequencerComposerView::~SequencerComposerView()
{
  WriteSettings();
}

void SequencerComposerView::SetModels(ApplicationModels* models)
{
  m_project_model = models->GetProjectModel();

  m_composer_actions->SetModel(models->GetSequencerModel());
  m_splittable_editor_widget->SetModel(models->GetSequencerModel());

  auto composer_info = m_project_model->Get<ComposerSettingsItem>()->GetComposerViewInfo();
  m_splittable_editor_widget->SetComposerViewInfo(composer_info);

  m_composer_tools_panel->SetModels(models);  // will select first procedure
}

void SequencerComposerView::ReadSettings()
{
  m_splittable_editor_widget->ReadSettings(sup::gui::GetSettingsReadFunc());
  m_splitter->ReadSettings();
}

void SequencerComposerView::WriteSettings()
{
  m_splittable_editor_widget->WriteSettings(sup::gui::GetSettingsWriteFunc());
  m_splitter->WriteSettings();

  // FIXME will crash if uncomment, since project model is destroyed even before our destructor
  // auto composer_info = m_splittable_editor_widget->GetComposerViewInfo();
  // m_project_model->Get<ComposerSettingsItem>()->SetComposerViewInfo(composer_info);
}

void SequencerComposerView::SetupConnections()
{
  // procedure selection request from toolbox to procedure editor
  auto on_procedure_selected = [this](const mvvm::ItemSelection& selection)
  {
    auto selected_procedure = selection.GetSelected<ProcedureItem>();
    m_splittable_editor_widget->SetProcedure(const_cast<ProcedureItem*>(selected_procedure));
    m_composer_actions->SetProcedure(const_cast<ProcedureItem*>(selected_procedure));
  };
  connect(m_composer_tools_panel, &ComposerToolsPanel::procedureSelectionChanged, this,
          on_procedure_selected);

  // propagate selection from focused procedure editor to toolbox
  connect(m_splittable_editor_widget,
          &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged,
          m_composer_tools_panel, &ComposerToolsPanel::SetProcedureSelection);

  // instruction toolbox requests form left panel
  connect(m_composer_tools_panel, &ComposerToolsPanel::ToolBoxInstructionRequest,
          m_splittable_editor_widget,
          &ProcedureSplittableEditorWidget::InsertInstructionFromToolBox);
}

void SequencerComposerView::SetupWidgetActions()
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
