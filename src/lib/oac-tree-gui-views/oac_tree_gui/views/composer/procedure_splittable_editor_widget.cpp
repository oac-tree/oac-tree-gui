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

#include "procedure_splittable_editor_widget.h"

#include "procedure_composer_tab_widget.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <QSplitter>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(const oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns appropriate procedure from the model by its identifier.
 *
 * If no procedure with such identifier exist, returns first procedure in the model.
 */
ProcedureItem* FindAppropriateProcedure(SequencerModel* model, const std::string& identifier)
{
  if (model == nullptr)
  {
    return nullptr;
  }

  if (auto procedure_item = dynamic_cast<ProcedureItem*>(model->FindItem(identifier));
      procedure_item)
  {
    return procedure_item;
  }

  auto procedures = model->GetProcedures();
  return procedures.empty() ? nullptr : procedures.front();
}

}  // namespace

ProcedureSplittableEditorWidget::ProcedureSplittableEditorWidget(
    sup::gui::IAppCommandService& command_service, sup::gui::ISettingsManager& settings_manager,
    QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_settings_manager(settings_manager)
    , m_splitter(new QSplitter)
    // , m_focus_handler(std::make_unique<WidgetFocusHandler<ProcedureComposerComboPanel>>())
    , m_splitter_controller(CreateSplitterController())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  connect(m_splitter_controller.get(),
          &sup::gui::SplittableEditorController::focusPanelSelectionChanged, this,
          &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  CreateInitialPanelsFromSettings();
  RegisterOwnSettingsInStore();
}

ProcedureSplittableEditorWidget::~ProcedureSplittableEditorWidget() = default;

void ProcedureSplittableEditorWidget::SetModel(SequencerModel* model)
{
  m_model = model;
  m_splitter_controller->SetModel(model);

  const auto view_info = m_settings_manager.Get<ComposerViewInfo>(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kApplication);
  const auto panels = m_splitter_controller->GetPanels();
  for (std::size_t index = 0; index < panels.size(); ++index)
  {
    const auto procedure_id = index < view_info.editor_info_list.size()
                                  ? view_info.editor_info_list[index].procedure_id
                                  : std::string();
    panels[index]->SetItem(FindAppropriateProcedure(m_model, procedure_id));
  }
}

void ProcedureSplittableEditorWidget::SetProcedure(ProcedureItem* procedure_item)
{
  if (auto focus_widget = m_splitter_controller->GetFocusWidget(); focus_widget)
  {
    focus_widget->SetItem(procedure_item);
  }
}

sup::gui::SplittableComboPanel* ProcedureSplittableEditorWidget::CreatePanel()
{
  return m_splitter_controller->CreatePanel();
}

void ProcedureSplittableEditorWidget::InsertInstructionFromToolBox(const QString& name)
{
  if (auto focus_widget = m_splitter_controller->GetFocusWidget(); focus_widget)
  {
    auto tab_widget = focus_widget->GetMainEditor<ProcedureComposerTabWidget>();
    tab_widget->InsertInstructionFromToolBox(name);
  }
}

ComposerViewInfo ProcedureSplittableEditorWidget::GetComposerViewInfo() const
{
  ComposerViewInfo info;
  if (m_splitter->count() > 1)
  {
    info.splitter_state = m_splitter->saveState().toBase64().toStdString();
  }

  for (auto& widget : m_splitter_controller->GetPanels())
  {
    ProcedureEditorInfo editor_info;
    if (auto procedure = widget->GetCurrentItem(); procedure != nullptr)
    {
      editor_info.procedure_id = procedure->GetIdentifier();
    }
    auto tab_widget = widget->GetMainEditor<ProcedureComposerTabWidget>();
    editor_info.editor_type = tab_widget->GetEditorType();
    info.editor_info_list.push_back(editor_info);
  }

  return info;
}

void ProcedureSplittableEditorWidget::SetComposerViewInfo(const ComposerViewInfo& view_info)
{
  // ClosePanel deletes later, the panel would stay in the splitter until then
  for (auto panel : m_splitter_controller->GetPanels())
  {
    m_splitter_controller->ClosePanel(panel);
    delete panel;
  }

  for (const auto& procedure_info : view_info.editor_info_list)
  {
    auto panel = CreatePanel();
    auto tab_widget = panel->GetMainEditor<ProcedureComposerTabWidget>();
    tab_widget->SetEditorType(procedure_info.editor_type);
    panel->SetItem(FindAppropriateProcedure(m_model, procedure_info.procedure_id));
  }

  // splitter state
  if (!view_info.splitter_state.empty())
  {
    m_splitter->restoreState(
        QByteArray::fromBase64(QByteArray::fromStdString(view_info.splitter_state)));
  }
}

sup::gui::SplittableEditorController* ProcedureSplittableEditorWidget::GetController() const
{
  return m_splitter_controller.get();
}

std::unique_ptr<sup::gui::SplittableEditorController>
ProcedureSplittableEditorWidget::CreateSplitterController() const
{
  auto factory_func = [this]() -> std::unique_ptr<sup::gui::SplittableComboPanel>
  {
    auto main_editor = std::make_unique<ProcedureComposerTabWidget>(m_command_service);

    auto item_list_callback = [this]() -> std::vector<mvvm::SessionItem*>
    {
      return m_model == nullptr
                 ? std::vector<mvvm::SessionItem*>()
                 : mvvm::utils::ConvertItems<mvvm::SessionItem>(m_model->GetProcedures());
    };

    auto result = std::make_unique<sup::gui::SplittableComboPanel>(std::move(main_editor),
                                                                   item_list_callback);
    result->SetModel(m_model);
    return result;
  };
  return std::make_unique<sup::gui::SplittableEditorController>(m_splitter, factory_func);
}

void ProcedureSplittableEditorWidget::CreateInitialPanelsFromSettings()
{
  // using persistent storage
  auto view_info = m_settings_manager.Get<ComposerViewInfo>(kComposerPanelsSettingsGroup,
                                                            sup::gui::SettingsScope::kApplication);
  if (view_info.editor_info_list.empty())
  {
    view_info.editor_info_list.emplace_back();
  }
  SetComposerViewInfo(view_info);
}

void ProcedureSplittableEditorWidget::RegisterOwnSettingsInStore()
{
  // registers own settings to report them to two different settings store
  auto provider = [this]() { return GetComposerViewInfo(); };
  m_settings_connections.push_back(m_settings_manager.RegisterProvider(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kApplication, provider));
  m_settings_connections.push_back(m_settings_manager.RegisterProvider(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kProject, provider));

  // subscribe to notification update in project setting store
  auto on_project_settings = [this](const ComposerViewInfo& info)
  {
    if (!info.editor_info_list.empty())
    {
      // a project without stored panels keeps the current ones
      SetComposerViewInfo(info);
    }
  };
  m_settings_connections.push_back(m_settings_manager.Subscribe<ComposerViewInfo>(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kProject, on_project_settings));
}

}  // namespace oac_tree_gui
