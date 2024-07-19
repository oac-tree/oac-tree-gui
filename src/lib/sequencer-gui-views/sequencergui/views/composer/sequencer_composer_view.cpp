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

#include "sequencer_composer_view.h"

#include "composer_panel.h"
#include "composer_widget_panel.h"
#include "sequencer_composer_actions.h"

#include <sequencergui/composer/procedure_plugin_controller.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/views/operation/procedure_action_handler.h>
#include <sup/gui/app/app_action_helper.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSettings>
#include <QSplitter>
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
SequencerComposerView::SequencerComposerView(QWidget *parent)
    : QWidget(parent)
    , m_composer_panel(new ComposerPanel)
    , m_central_panel(new ComposerWidgetPanel)
    , m_right_panel(new ComposerWidgetPanel)
    , m_splitter(new QSplitter)
    , m_composer_actions(new SequencerComposerActions(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_central_panel);
  m_splitter->addWidget(m_right_panel);

  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(40) << mvvm::utils::UnitSize(80)
                                    << mvvm::utils::UnitSize(80));

  layout->addWidget(m_splitter);

  SetupConnections();

  ReadSettings();

  m_composer_actions->RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(this));
}

SequencerComposerView::~SequencerComposerView()
{
  WriteSettings();
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_panel->SetModel(model);
  m_central_panel->SetModel(model);
  m_right_panel->SetModel(model);

  m_plugin_controller = std::make_unique<ProcedurePluginController>(model);
}

void SequencerComposerView::OnProjectLoad()
{
  m_composer_panel->SetSelectedProcedure(GetFirstProcedure());
}

void SequencerComposerView::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }

  auto central_index = settings.value(kCentralPanel, ComposerWidgetPanel::kInstructionTree).toInt();
  m_central_panel->SetCurrentWidgetType(
      static_cast<ComposerWidgetPanel::WidgetType>(central_index));

  auto right_index = settings.value(kRightPanel, ComposerWidgetPanel::kWorkspace).toInt();
  m_right_panel->SetCurrentWidgetType(static_cast<ComposerWidgetPanel::WidgetType>(right_index));
}

void SequencerComposerView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
  settings.setValue(kCentralPanel, m_central_panel->GetCurrentWidgetType());
  settings.setValue(kRightPanel, m_right_panel->GetCurrentWidgetType());
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

//! Returns first procedure from the procedure container, if exist.
ProcedureItem *SequencerComposerView::GetFirstProcedure()
{
  auto procedure_container = m_model->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequencergui
