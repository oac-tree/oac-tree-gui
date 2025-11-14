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

#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/style/style_helper.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QAction>
#include <QComboBox>

namespace oac_tree_gui
{

ComposerComboPanelToolBar::ComposerComboPanelToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget)
    , m_context_selector(new QComboBox)
    , m_split_horizontally_action(new QAction)
    , m_close_current_view_action(new QAction)
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  addWidget(m_context_selector);

  m_context_selector->setFixedWidth(mvvm::style::UnitSize(16));

  InsertStretch();

  m_split_horizontally_action = new QAction("Split", this);
  m_split_horizontally_action->setToolTip("Split current view to the right");
  m_split_horizontally_action->setIcon(FindIcon("view-split-left-right-16"));
  connect(m_split_horizontally_action, &QAction::triggered, this,
          &ComposerComboPanelToolBar::splitViewRequest);
  addAction(m_split_horizontally_action);

  m_close_current_view_action = new QAction("Close", this);
  m_close_current_view_action->setToolTip("Close this view");
  m_close_current_view_action->setIcon(FindIcon("dialog-close-16"));
  connect(m_close_current_view_action, &QAction::triggered, this,
          &ComposerComboPanelToolBar::closeViewRequest);
  addAction(m_close_current_view_action);

  // make it narrow, buttons closer to each other
  // setStyleSheet(
  //     "QToolBar { padding-bottom: 0px; margin-bottom: 0px; } QToolButton { padding: 0px; margin:
  //     " "0px; }");

  setStyleSheet("QToolButton { padding: 0px; margin:0px; }");
}

ComposerComboPanelToolBar::~ComposerComboPanelToolBar() = default;

void ComposerComboPanelToolBar::InsertStretch()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace oac_tree_gui
