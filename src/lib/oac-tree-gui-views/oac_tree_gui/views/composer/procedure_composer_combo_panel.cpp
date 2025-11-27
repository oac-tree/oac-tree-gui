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

#include "procedure_composer_combo_panel.h"

#include "procedure_composer_combo_toolbar.h"
#include "procedure_composer_tab_widget.h"
#include "procedure_editor_placeholder.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QMouseEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

ProcedureComposerComboPanel::ProcedureComposerComboPanel(
    sup::gui::IAppCommandService& command_service, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ProcedureComposerComboToolBar([this]() { return m_model->GetProcedures(); }))
    , m_stacked_widget(new QStackedWidget)
    , m_placeholder_widget(new ProcedureEditorPlaceholder)
    , m_procedure_composer_widget(new ProcedureComposerTabWidget(command_service))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_stacked_widget);

  m_stacked_widget->addWidget(m_placeholder_widget);
  m_stacked_widget->addWidget(m_procedure_composer_widget);

  SetPlaceholderMode(PlaceholderMode::kOn);
  SetupConnections();
}

ProcedureComposerComboPanel::ProcedureComposerComboPanel(
    sup::gui::IAppCommandService& command_service, SequencerModel* model, QWidget* parent_widget)
    : ProcedureComposerComboPanel(command_service, parent_widget)
{
  SetModel(model);
}

ProcedureComposerComboPanel::~ProcedureComposerComboPanel() = default;

SequencerModel* ProcedureComposerComboPanel::GetModel() const
{
  return m_model;
}

void ProcedureComposerComboPanel::SetModel(SequencerModel* model)
{
  m_model = model;
  if (m_model)
  {
    SetupModelListener();
  }
  else
  {
    m_listener.reset();
  }
}

void ProcedureComposerComboPanel::SetProcedure(ProcedureItem* procedure_item)
{
  m_tool_bar->UpdateProcedureSelectionMenu(procedure_item);
  SetProcedureIntern(procedure_item);
}

ProcedureItem* ProcedureComposerComboPanel::GetCurrentProcedure() const
{
  return m_current_procedure_item;
}

void ProcedureComposerComboPanel::ShowAsActive(bool value)
{
  m_tool_bar->ShowAsActive(value);
}

void ProcedureComposerComboPanel::ShowAsLastEditor(bool value)
{
  m_tool_bar->ShowAsLastEditor(value);
}

ProcedureEditorType ProcedureComposerComboPanel::GetProcedureEditorType() const
{
  return m_procedure_composer_widget->GetEditorType();
}

void ProcedureComposerComboPanel::SetProcedureEditorType(ProcedureEditorType editor_type)
{
  m_procedure_composer_widget->SetEditorType(editor_type);
}

void ProcedureComposerComboPanel::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    // Here we use a simple yet reliable way to track focus while user is switching between panels.
    // We report every click as a kind of focus request to WidgetFocusHandler. He will perform
    // necessary checks and updates.
    emit panelFocusRequest();
  }

  QWidget::mousePressEvent(event);
}

void ProcedureComposerComboPanel::SetupConnections()
{
  connect(m_tool_bar, &ProcedureComposerComboToolBar::splitViewRequest, this,
          &ProcedureComposerComboPanel::splitViewRequest);
  connect(m_tool_bar, &ProcedureComposerComboToolBar::closeViewRequest, this,
          &ProcedureComposerComboPanel::closeViewRequest);
  connect(m_tool_bar, &ProcedureComposerComboToolBar::selectProcedureRequest, this,
          &ProcedureComposerComboPanel::SetProcedureIntern);
}

void ProcedureComposerComboPanel::SetupModelListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_model);

  auto on_data_changed = [this](const mvvm::DataChangedEvent& event)
  {
    if (event.item == m_current_procedure_item)
    {
      m_tool_bar->UpdateProcedureSelectionMenu(m_current_procedure_item);
    }
  };
  m_listener->Connect<mvvm::DataChangedEvent>(on_data_changed);

  auto on_about_to_remove_item = [this](const mvvm::AboutToRemoveItemEvent& event)
  {
    auto item_to_remove = event.item->GetItem(event.tag_index);

    if ((item_to_remove == m_current_procedure_item)
        || mvvm::utils::IsItemAncestor(m_current_procedure_item, item_to_remove))
    {
      SetProcedure(nullptr);
    }
  };
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(on_about_to_remove_item);
}

void ProcedureComposerComboPanel::SetProcedureIntern(ProcedureItem* item)
{
  if (m_current_procedure_item == item)
  {
    return;
  }

  m_current_procedure_item = item;

  m_procedure_composer_widget->SetProcedure(m_current_procedure_item);
  SetPlaceholderMode(item == nullptr ? PlaceholderMode::kOn : PlaceholderMode::kOff);
  emit selectedProcedureChanged(m_current_procedure_item);
}

void ProcedureComposerComboPanel::SetPlaceholderMode(PlaceholderMode mode)
{
  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(mode));
}

}  // namespace oac_tree_gui
