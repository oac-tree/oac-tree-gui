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

#include <QDebug>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace oac_tree_gui
{

ComposerComboPanel::ComposerComboPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComposerComboPanelToolBar({}))
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

  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(WidgetType::kPlaceholderWidget));

  SetupConnections();
}

ComposerComboPanel::~ComposerComboPanel() = default;

void ComposerComboPanel::mousePressEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton)
  {
    return;
  }
  setFocus(Qt::MouseFocusReason);
  QWidget::mousePressEvent(event);
}

void ComposerComboPanel::focusInEvent(QFocusEvent* event)
{
  qDebug() << "focusInEvent" << (this);
  QWidget::focusInEvent(event);
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

  const auto index = static_cast<std::int32_t>(
      item == nullptr ? WidgetType::kPlaceholderWidget : WidgetType::kProcedureComposerWidget);
  m_stacked_widget->setCurrentIndex(index);
}

}  // namespace oac_tree_gui
