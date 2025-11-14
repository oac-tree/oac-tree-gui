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

#include <QStackedWidget>
#include <QVBoxLayout>

namespace oac_tree_gui
{

ComposerComboPanel::ComposerComboPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComposerComboPanelToolBar)
    , m_stacked_widget(new QStackedWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_stacked_widget);
}

ComposerComboPanel::~ComposerComboPanel() = default;

}  // namespace oac_tree_gui
