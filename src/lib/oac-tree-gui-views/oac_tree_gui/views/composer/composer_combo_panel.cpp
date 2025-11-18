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

#include <QDebug>
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
  if (event->button() != Qt::LeftButton)
  {
    return;
  }

  // Seems this is a robust way to set focus on mouse click without messing with focus stealing by
  // the child widget.

  setFocus(Qt::MouseFocusReason);
  QWidget::mousePressEvent(event);
}

void ComposerComboPanel::focusInEvent(QFocusEvent* event)
{
  qDebug() << "focusInEvent" << (this) << event->reason();
  if (event->reason() == Qt::MouseFocusReason)
  {
    emit panelFocusRequest();
    // m_tool_bar->ShowAsActive(true);
  }
  QWidget::focusInEvent(event);
}

void ComposerComboPanel::focusOutEvent(QFocusEvent* event)
{
  qDebug() << "focusOutEvent" << (this) << event->reason();
  if (event->reason() == Qt::MouseFocusReason)
  {
    // m_tool_bar->ShowAsActive(false);
  }
  QWidget::focusOutEvent(event);
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
  m_procedure_composer_widget->SetProcedure(item);
  ShowWidget(item == nullptr ? WidgetType::kPlaceholderWidget
                             : WidgetType::kProcedureComposerWidget);
}

void ComposerComboPanel::ShowWidget(WidgetType widget_type)
{
  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(widget_type));
}

}  // namespace oac_tree_gui
