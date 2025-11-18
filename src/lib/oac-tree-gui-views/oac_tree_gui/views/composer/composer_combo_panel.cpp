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

#include "composer_combo_panel.h"

#include "composer_combo_panel_toolbar.h"
#include "placeholder_widget.h"
#include "procedure_composer_tab_widget.h"

#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QMouseEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui
{

ComposerComboPanel::ComposerComboPanel(const ProceduresCallback& procedure_callback,
                                       QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComposerComboPanelToolBar(procedure_callback))
    , m_stacked_widget(new QStackedWidget)
    , m_placeholder_widget(new PlaceholderWidget("ABC"))
    , m_procedure_composer_widget(new ProcedureComposerTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_stacked_widget);

  m_stacked_widget->addWidget(m_placeholder_widget);
  m_stacked_widget->addWidget(m_procedure_composer_widget);

  ShowWidget(WidgetType::kPlaceholderWidget);
  SetupConnections();
}

ComposerComboPanel::~ComposerComboPanel() = default;

void ComposerComboPanel::SetProcedure(ProcedureItem* procedure_item)
{
  m_tool_bar->UpdateProcedureSelectionMenu(procedure_item);
  OnSelectProcedureRequest(procedure_item);
}

void ComposerComboPanel::ShowAsActive(bool value)
{
  m_tool_bar->ShowAsActive(value);
}

void ComposerComboPanel::ShowAsLastEditor(bool value)
{
  m_tool_bar->ShowAsLastEditor(value);
}

void ComposerComboPanel::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    // I didn't find a reliable way to track focus events. We pretend every click in a widget
    // denotes the user interest in that widget, notify the manager, and let him handle the rest.
    emit panelFocusRequest();
  }

  QWidget::mousePressEvent(event);
}

void ComposerComboPanel::SetupConnections()
{
  connect(m_tool_bar, &ComposerComboPanelToolBar::splitViewRequest, this,
          &ComposerComboPanel::splitViewRequest);
  connect(m_tool_bar, &ComposerComboPanelToolBar::closeViewRequest, this,
          &ComposerComboPanel::closeViewRequest);
  connect(m_tool_bar, &ComposerComboPanelToolBar::SelectProcedureRequest, this,
          &ComposerComboPanel::OnSelectProcedureRequest);
}

void ComposerComboPanel::OnSelectProcedureRequest(ProcedureItem* item)
{
  if (m_current_procedure_item == item)
  {
    return;
  }

  m_current_procedure_item = item;

  m_procedure_composer_widget->SetProcedure(m_current_procedure_item);
  ShowWidget(item == nullptr ? WidgetType::kPlaceholderWidget
                             : WidgetType::kProcedureComposerWidget);
  InitModelListener();
}

void ComposerComboPanel::ShowWidget(WidgetType widget_type)
{
  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(widget_type));
}

void ComposerComboPanel::InitModelListener()
{
  if (!m_current_procedure_item)
  {
    m_listener.reset();
    return;
  }

  m_listener = std::make_unique<mvvm::ModelListener>(m_current_procedure_item->GetModel());

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

}  // namespace oac_tree_gui
