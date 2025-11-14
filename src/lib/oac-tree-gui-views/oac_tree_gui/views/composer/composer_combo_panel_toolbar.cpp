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

#include "composer_combo_panel_toolbar.h"

#include <sup/gui/style/style_helper.h>

#include <QComboBox>

namespace oac_tree_gui
{

ComposerComboPanelToolBar::ComposerComboPanelToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget), m_context_selector(new QComboBox)
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  addWidget(m_context_selector);

  InsertStretch();
}

ComposerComboPanelToolBar::~ComposerComboPanelToolBar() = default;

void ComposerComboPanelToolBar::InsertStretch()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace oac_tree_gui
