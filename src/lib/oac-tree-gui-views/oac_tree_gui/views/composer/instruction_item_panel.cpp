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

#include "instruction_item_panel.h"

#include "instruction_toolkit_tree_view.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/domain_object_group_helper.h>
#include <oac_tree_gui/viewmodel/toolkit_viewmodel.h>

#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <QAction>
#include <QLineEdit>
#include <QMenu>
#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionItemPanel::InstructionItemPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_instruction_toolkit_viewmodel(new ToolKitViewModel(this))
    , m_proxy_model(new mvvm::FilterNameViewModel(this))
    , m_tree_view(new InsructionToolKitTreeView)
    , m_line_edit(new QLineEdit)
{
  setWindowTitle("INSTRUCTIONS");

  m_proxy_model->setSourceModel(m_instruction_toolkit_viewmodel);
  m_proxy_model->setRecursiveFilteringEnabled(true);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
  layout->addWidget(m_line_edit);

  connect(m_tree_view, &InsructionToolKitTreeView::InstructionDoubleClicked, this,
          &InstructionItemPanel::InstructionDoubleClicked);

  m_instruction_toolkit_viewmodel->PopulateModel(CreateInstructionTypeGroups());

  m_tree_view->setModel(m_proxy_model);
  m_tree_view->expandAll();
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &InstructionItemPanel::OnContextMenuRequest);

  m_line_edit->setClearButtonEnabled(true);
  m_line_edit->setPlaceholderText("Find instruction");
  auto on_text = [this]() { m_proxy_model->SetPattern(m_line_edit->text()); };

  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);
}

void InstructionItemPanel::OnContextMenuRequest(const QPoint& point)
{
  QMenu menu;

  auto expand_all_action = menu.addAction("Expand all");
  connect(expand_all_action, &QAction::triggered, m_tree_view, &QTreeView::expandAll);
  auto collapse_all_action = menu.addAction("Collapse all");
  connect(collapse_all_action, &QAction::triggered, m_tree_view, &QTreeView::collapseAll);

  menu.exec(m_tree_view->mapToGlobal(point));
}

}  // namespace oac_tree_gui
