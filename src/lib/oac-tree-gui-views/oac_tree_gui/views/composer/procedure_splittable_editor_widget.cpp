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

#include "procedure_composer_combo_panel.h"

#include <oac_tree_gui/composer/widget_focus_handler.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <QSplitter>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

namespace
{

constexpr auto kGroupName = "ProcedureSplittableEditorWidget";

/**
 * @brief Returns QSettings key holding the number of created panels
 */
QString GetPanelCountKey()
{
  return QString(kGroupName) + "/panel_count";
}

/**
 * @brief Returns QSettings key holding the state of the splitter.
 */
QString GetSplitterStateKey()
{
  return QString(kGroupName) + "/splitter";
}

/**
 * @brief Returns QSettings key holding the state of the splitter.
 */
QString GetProcedureEditorTabStateKey()
{
  return QString(kGroupName) + "/procedure_tabs";
}

}  // namespace

ProcedureSplittableEditorWidget::ProcedureSplittableEditorWidget(
    sup::gui::IAppCommandService& command_service, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_splitter(new QSplitter)
    , m_focus_handler(std::make_unique<WidgetFocusHandler<ProcedureComposerComboPanel>>())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);
}

ProcedureSplittableEditorWidget::~ProcedureSplittableEditorWidget() = default;

void ProcedureSplittableEditorWidget::SetModel(SequencerModel* model)
{
  m_model = model;
  for (auto& widget : m_focus_handler->GetWidgets())
  {
    widget->SetModel(m_model);
  }
}

void ProcedureSplittableEditorWidget::SetProcedure(ProcedureItem* procedure_item)
{
  if (auto focus_widget = m_focus_handler->GetFocusWidget(); focus_widget)
  {
    focus_widget->SetProcedure(procedure_item);
  }
}

void ProcedureSplittableEditorWidget::CreatePanel(ProcedureComposerComboPanel* after_widget)
{
  auto new_widget = CreateProcedureEditor();
  auto new_widget_ptr = new_widget.get();

  if (after_widget == nullptr)
  {
    m_splitter->addWidget(new_widget.release());
    m_focus_handler->AddWidget(new_widget_ptr);
  }
  else
  {
    const std::int32_t index = m_splitter->indexOf(after_widget);
    m_splitter->insertWidget(index + 1, new_widget.release());
    m_focus_handler->AddWidget(new_widget_ptr, after_widget);
    new_widget_ptr->SetProcedure(after_widget->GetCurrentProcedure());
  }
}

void ProcedureSplittableEditorWidget::ClosePanel(ProcedureComposerComboPanel* widget_to_close)
{
  if (widget_to_close == nullptr)
  {
    return;
  }
  m_focus_handler->RemoveWidget(widget_to_close);
  widget_to_close->hide();
  widget_to_close->deleteLater();
}

ProcedureComposerComboPanel* ProcedureSplittableEditorWidget::GetFocusWidget()
{
  return m_focus_handler->GetFocusWidget();
}

ProcedureComposerComboPanel* ProcedureSplittableEditorWidget::GetWidgetAt(std::size_t index)
{
  return m_focus_handler->GetWidgetAt(index);
}

void ProcedureSplittableEditorWidget::SetFocusWidget(ProcedureComposerComboPanel* widget)
{
  m_focus_handler->SetFocusWidget(widget);
  if (widget != nullptr)
  {
    NotifyFocusWidgetProcedureSelectionChanged(widget->GetCurrentProcedure());
  }
}

void ProcedureSplittableEditorWidget::ReadSettings(const sup::gui::read_variant_func_t& read_func)
{
  if (m_splitter->count() > 0)
  {
    // already initialized
    return;
  }

  // number of panels
  const auto panel_count_variant = read_func(GetPanelCountKey());
  const auto panel_count = panel_count_variant.isValid() ? panel_count_variant.toInt() : 1;
  for (std::int32_t i = 0; i < panel_count; ++i)
  {
    CreatePanel();
  }

  // splitter state
  auto splitter_state = read_func(GetSplitterStateKey());
  if (splitter_state.isValid())
  {
    m_splitter->restoreState(splitter_state.toByteArray());
  }

  // active tabs in panels
  const auto tab_states_variant = read_func(GetProcedureEditorTabStateKey());
  if (tab_states_variant.isValid())
  {
    const auto widget_tabs = tab_states_variant.value<QList<std::int32_t>>();
    std::int32_t widget_index{0};
    for (auto& widget : m_focus_handler->GetWidgets())
    {
      if (widget_index >= widget_tabs.size())
      {
        break;
      }
      const auto tab_index = widget_tabs.at(widget_index);
      widget->SetProcedureEditorType(static_cast<ProcedureEditorType>(tab_index));
      ++widget_index;
    }
  }
}

void ProcedureSplittableEditorWidget::WriteSettings(
    const sup::gui::write_variant_func_t& write_func)
{
  const auto panel_count = static_cast<std::int32_t>(m_focus_handler->GetCount());
  write_func(GetPanelCountKey(), QVariant::fromValue(panel_count));
  write_func(GetSplitterStateKey(), m_splitter->saveState());

  QList<std::int32_t> tab_indexes;
  for (auto& widget : m_focus_handler->GetWidgets())
  {
    tab_indexes.append(static_cast<std::int32_t>(widget->GetProcedureEditorType()));
  }
  write_func(GetProcedureEditorTabStateKey(), QVariant::fromValue(tab_indexes));
}

void ProcedureSplittableEditorWidget::InsertInstructionFromToolBox(const QString& name)
{
  if (auto focus_widget = m_focus_handler->GetFocusWidget(); focus_widget)
  {
    focus_widget->InsertInstructionFromToolBox(name);
  }
}

std::unique_ptr<ProcedureComposerComboPanel>
ProcedureSplittableEditorWidget::CreateProcedureEditor()
{
  auto result = std::make_unique<ProcedureComposerComboPanel>(m_command_service, m_model);

  auto on_add_panel = [this]()
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    CreatePanel(/*add after*/ sending_panel);
  };
  connect(result.get(), &ProcedureComposerComboPanel::splitViewRequest, this, on_add_panel);

  auto on_remove_panel = [this]()
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    ClosePanel(sending_panel);
  };
  connect(result.get(), &ProcedureComposerComboPanel::closeViewRequest, this, on_remove_panel);

  auto on_focus_request = [this]()
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    m_focus_handler->SetFocusWidget(sending_panel);
    NotifyFocusWidgetProcedureSelectionChanged(sending_panel->GetCurrentProcedure());
  };
  connect(result.get(), &ProcedureComposerComboPanel::panelFocusRequest, this, on_focus_request);

  auto on_selected_procedure_changed = [this](oac_tree_gui::ProcedureItem* item)
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());

    if (sending_panel == GetFocusWidget())
    {
      NotifyFocusWidgetProcedureSelectionChanged(item);
    }
  };
  connect(result.get(), &ProcedureComposerComboPanel::selectedProcedureChanged, this,
          on_selected_procedure_changed);

  return result;
}

void ProcedureSplittableEditorWidget::NotifyFocusWidgetProcedureSelectionChanged(
    ProcedureItem* item)
{
  if (m_block_selection_change_notification)
  {
    return;
  }

  m_block_selection_change_notification = true;

  if (item != m_procedure_item_in_focus_cache)
  {
    m_procedure_item_in_focus_cache = item;
    emit focusWidgetProcedureSelectionChanged(item);
  }

  m_block_selection_change_notification = false;
}

}  // namespace oac_tree_gui
