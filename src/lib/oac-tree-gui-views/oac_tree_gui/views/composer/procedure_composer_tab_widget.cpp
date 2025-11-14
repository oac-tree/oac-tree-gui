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

#include "procedure_composer_tab_widget.h"

#include <QTabWidget>
#include <QVBoxLayout>

namespace oac_tree_gui
{

ProcedureComposerTabWidget::ProcedureComposerTabWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tab_widget);

  m_tab_widget->addTab(new QWidget, "ABC");
  m_tab_widget->addTab(new QWidget, "DEF");
  m_tab_widget->setTabPosition(QTabWidget::South);
}

ProcedureComposerTabWidget::~ProcedureComposerTabWidget() = default;

}  // namespace oac_tree_gui
