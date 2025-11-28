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

#include "realtime_instruction_tree_widget.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/operation/objects/breakpoint_model_delegate.h>
#include <oac_tree_gui/operation/objects/instruction_tree_expand_controller.h>
#include <oac_tree_gui/operation/tooltip_helper.h>
#include <oac_tree_gui/viewmodel/instruction_operation_viewmodel.h>

#include <sup/gui/components/item_filter_helper.h>
#include <sup/gui/components/tree_helper.h>
#include <sup/gui/core/environment.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/views/component_provider_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QEvent>
#include <QHelpEvent>
#include <QMenu>
#include <QSettings>
#include <QToolTip>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kHeaderStateSettingName("RealTimeInstructionTreeWidget/header_state");
const std::vector<int> kDefaultColumnStretch({15, 5, 1});

QString GetCustomToolTipStyle()
{
  static const QString style(
      "QToolTip {border: 2px solid darkgrey; padding: 5px; border-radius: 3px;}");

  // Adwaita style has own white-on-black semi-transparent tooltip style.
  // We have to define dark background to match this style.
  static const QString adwaita_style(
      "QToolTip {border: 2px solid lightgray; padding: 5px; border-radius: 3px; background-color: "
      "darkgray}");

  return sup::gui::IsOnCodac() ? adwaita_style : style;
}

}  // namespace

namespace oac_tree_gui
{

RealTimeInstructionTreeWidget::RealTimeInstructionTreeWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<InstructionOperationViewModel>(m_tree_view))
    , m_custom_header(
          new sup::gui::CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_delegate(std::make_unique<BreakpointModelDelegate>())
    , m_expand_controller(std::make_unique<InstructionTreeExpandController>(m_tree_view))
{
  setWindowTitle("InstructionTree");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  m_tree_view->setItemDelegate(m_delegate.get());
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &RealTimeInstructionTreeWidget::OnCustomContextMenuRequested);

  connect(m_tree_view, &QTreeView::doubleClicked, this,
          &RealTimeInstructionTreeWidget::OnTreeDoubleClick);

  auto on_branch_change = [this]()
  {
    m_component_provider->SetSelectedItems(m_expand_controller->GetInstructionsToSelect());
    ScrollViewportToSelection();
  };
  connect(m_expand_controller.get(), &InstructionTreeExpandController::VisibilityHasChanged, this,
          on_branch_change);

  setStyleSheet(GetCustomToolTipStyle());
}

RealTimeInstructionTreeWidget::~RealTimeInstructionTreeWidget() = default;

void RealTimeInstructionTreeWidget::SetProcedure(ProcedureItem* procedure_item)
{
  m_procedure = procedure_item;

  auto container =
      (procedure_item != nullptr) ? procedure_item->GetInstructionContainer() : nullptr;

  m_component_provider->SetItem(container);
  m_expand_controller->SetInstructionContainer(container);

  if (procedure_item != nullptr)
  {
    m_expand_controller->SetTreeViewToInstructionExpandState();
    m_custom_header->AdjustColumnsWidth();
  }
}

void RealTimeInstructionTreeWidget::SetSelectedInstructions(std::vector<InstructionItem*> items)
{
  m_expand_controller->SaveSelectionRequest(items);
  m_component_provider->SetSelectedItems(m_expand_controller->GetInstructionsToSelect());
  ScrollViewportToSelection();
}

void RealTimeInstructionTreeWidget::SetViewportFollowsSelectionFlag(bool value)
{
  m_viewport_follows_selection = value;
  ScrollViewportToSelection();
}

bool RealTimeInstructionTreeWidget::event(QEvent* event)
{
  if (event->type() == QEvent::ToolTip)
  {
    auto global_pos = static_cast<QHelpEvent*>(event)->globalPos();
    auto pos = m_tree_view->viewport()->mapFromGlobal(global_pos);
    auto index = m_tree_view->indexAt(pos);
    auto item = m_component_provider->GetViewModel()->GetSessionItemFromIndex(index);
    auto text = GetInstructionToolTipText(item);
    if (text.isEmpty())
    {
      QToolTip::hideText();
      event->ignore();
    }
    else
    {
      QToolTip::showText(global_pos, text, this);
    }
  }
  return QWidget::event(event);
}

void RealTimeInstructionTreeWidget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  m_custom_header->AdjustColumnsWidth();
}

void RealTimeInstructionTreeWidget::OnTreeDoubleClick(const QModelIndex& index)
{
  if (index.column() == InstructionOperationViewModel::GetBreakpointColumn())
  {
    auto instruction = m_component_provider->GetSelected<InstructionItem>();
    emit ToggleBreakpointRequest(instruction);
  }
}

void RealTimeInstructionTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  QMenu menu;

  // setting up menu section with standard collapse/expand block
  sup::gui::SetupCollapseExpandMenu(pos, menu, *m_tree_view);
  menu.addSeparator();

  // setting menu with "selective" expand option
  auto selective_expand_action = menu.addAction("Selective expand");
  selective_expand_action->setToolTip(
      "Expand all except instructions with property 'show as collapsed' set.");

  auto on_action = [this]() { m_expand_controller->SetTreeViewToInstructionExpandState(); };
  QObject::connect(selective_expand_action, &QAction::triggered, this, on_action);

  menu.exec(m_tree_view->mapToGlobal(pos));
}

void RealTimeInstructionTreeWidget::ScrollViewportToSelection()
{
  if (!m_viewport_follows_selection)
  {
    return;
  }

  auto filtered = sup::gui::GetBottomLevelSelection(m_component_provider->GetSelectedItems());
  if (!filtered.empty())
  {
    auto indexes = m_component_provider->GetViewIndexes(filtered.front());
    sup::gui::ScrollTreeViewportToIndex(indexes.front(), *m_tree_view);
  }
}

}  // namespace oac_tree_gui
