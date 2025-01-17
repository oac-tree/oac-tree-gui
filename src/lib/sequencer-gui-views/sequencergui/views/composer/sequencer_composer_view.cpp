/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_composer_view.h"

#include "composer_panel.h"
#include "composer_widget_panel.h"
#include "sequencer_composer_actions.h"

#include <sequencergui/components/component_helper.h>
#include <sequencergui/composer/procedure_plugin_controller.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/style/style_helper.h>
#include <sequencergui/views/operation/procedure_action_handler.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/widgets/custom_splitter.h>

#include <mvvm/standarditems/container_item.h>

#include <QAction>
#include <QVBoxLayout>

namespace
{

const QString kGroupName("SequencerComposerView");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kCentralPanel = kGroupName + "/" + "central";
const QString kRightPanel = kGroupName + "/" + "right";

}  // namespace

namespace sequencergui
{
SequencerComposerView::SequencerComposerView(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_composer_panel(new ComposerPanel)
    , m_central_panel(new ComposerWidgetPanel(kCentralPanel, ComposerWidgetPanel::kInstructionTree))
    , m_right_panel(new ComposerWidgetPanel(kRightPanel, ComposerWidgetPanel::kWorkspace))
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
    , m_composer_actions(new SequencerComposerActions(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_central_panel);
  m_splitter->addWidget(m_right_panel);
  m_splitter->setSizes({200, 400, 400});

  layout->addWidget(m_splitter);

  SetupConnections();
  SetupWidgetActions();

  m_right_panel->setVisible(false);

  ReadSettings();
  RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(this));
}

SequencerComposerView::~SequencerComposerView()
{
  WriteSettings();
  sup::gui::AppUnregisterWidgetUniqueId(this);
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_actions->SetModel(model);
  m_composer_panel->SetModel(model);
  m_central_panel->SetModel(model);
  m_right_panel->SetModel(model);

  m_plugin_controller = std::make_unique<ProcedurePluginController>(model);
}

void SequencerComposerView::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  m_composer_actions->RegisterActionsForContext(context);
  sup::gui::AppAddActionToCommand(m_toggle_left_sidebar, constants::kToggleLeftPanelCommandId,
                                  context);
  sup::gui::AppAddActionToCommand(m_toggle_right_sidebar, constants::kToggleRightPanelCommandId,
                                  context);
}

void SequencerComposerView::ReadSettings()
{
  m_central_panel->ReadSettings();
  m_right_panel->ReadSettings();
  m_splitter->ReadSettings();
}

void SequencerComposerView::WriteSettings()
{
  m_central_panel->WriteSettings();
  m_right_panel->WriteSettings();
  m_splitter->WriteSettings();
}

void SequencerComposerView::SetupConnections()
{
  auto on_procedure_selected = [this](auto procedure_item)
  {
    m_central_panel->SetProcedure(procedure_item);
    m_right_panel->SetProcedure(procedure_item);
    m_composer_actions->SetProcedure(procedure_item);
  };
  connect(m_composer_panel, &ComposerPanel::ProcedureSelected, this, on_procedure_selected);

  // propagate selection from central panel to the right panel
  auto on_central_selection = [this](auto)
  { m_right_panel->SetSelectedInstructions(m_central_panel->GetSelectedInstructions()); };
  connect(m_central_panel, &ComposerWidgetPanel::InstructionSelected, this, on_central_selection);

  // propagate selection from right panel to the central panel
  auto on_right_selection = [this](auto)
  { m_central_panel->SetSelectedInstructions(m_right_panel->GetSelectedInstructions()); };
  connect(m_right_panel, &ComposerWidgetPanel::InstructionSelected, this, on_right_selection);

  // on left export
  auto on_export = [this]()
  {
    ProcedureActionHandler handler;
    handler.OnExportToXmlRequest(m_composer_panel->GetSelectedProcedure());
  };
  connect(m_central_panel, &ComposerWidgetPanel::ExportToFileRequest, this, on_export);
  connect(m_right_panel, &ComposerWidgetPanel::ExportToFileRequest, this, on_export);
}

void SequencerComposerView::SetupWidgetActions()
{
  m_toggle_left_sidebar = new QAction("Show/hide left sidebar", this);
  m_toggle_left_sidebar->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar->setIcon(FindIcon("dock-left"));
  connect(m_toggle_left_sidebar, &QAction::triggered, this,
          [this](auto) { m_composer_panel->setVisible(!m_composer_panel->isVisible()); });

  m_toggle_right_sidebar = new QAction("Show/hide right sidebar", this);
  m_toggle_right_sidebar->setToolTip("Show/hide right panel");
  m_toggle_right_sidebar->setIcon(FindIcon("dock-right"));
  connect(m_toggle_right_sidebar, &QAction::triggered, this,
          [this](auto) { m_right_panel->setVisible(!m_right_panel->isVisible()); });
}

}  // namespace sequencergui
