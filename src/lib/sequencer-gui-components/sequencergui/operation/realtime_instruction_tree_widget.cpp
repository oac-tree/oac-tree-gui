/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "realtime_instruction_tree_widget.h"

#include "breakpoint_model_delegate.h"
#include "instruction_tree_expand_controller.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/viewmodel/instruction_operation_viewmodel.h>
#include <sequencergui/widgets/style_utils.h>
#include <sequencergui/widgets/tree_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("RealTimeInstructionTreeWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";

bool FixColumnAppearance(QTreeView *tree)
{
  const int breakpoint_colum = sequencergui::InstructionOperationViewModel::GetBreakpointColumn();
  tree->header()->setSectionResizeMode(breakpoint_colum, QHeaderView::Fixed);
  tree->setColumnWidth(breakpoint_colum, mvvm::utils::UnitSize(1));
  return true;
}

}  // namespace

namespace sequencergui
{

RealTimeInstructionTreeWidget::RealTimeInstructionTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<InstructionOperationViewModel>(m_tree_view))
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_delegate(std::make_unique<BreakpointModelDelegate>())
      , m_selection_controller(std::make_unique<InstructionTreeExpandController>(m_tree_view))
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

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view);

  ReadSettings();
}

RealTimeInstructionTreeWidget::~RealTimeInstructionTreeWidget()
{
  WriteSettings();
}

void RealTimeInstructionTreeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure = procedure_item;

  auto container = procedure_item ? procedure_item->GetInstructionContainer() : nullptr;

  m_component_provider->SetItem(container);
  m_selection_controller->SetInstructionContainer(container);

  if (procedure_item)
  {
    AdjustTreeAppearance();
  }
}

void RealTimeInstructionTreeWidget::SetSelectedInstructions(std::vector<InstructionItem *> items)
{
  m_selection_controller->SaveSelectionRequest(items);
  m_component_provider->SetSelectedItems(m_selection_controller->GetInstructionsToSelect());
  ScrollViewportToSelection();
}

void RealTimeInstructionTreeWidget::SetViewportFollowsSelectionFlag(bool value)
{
  m_viewport_follows_selection = value;
  ScrollViewportToSelection();
}

void RealTimeInstructionTreeWidget::ReadSettings()
{
  const QSettings settings;
  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void RealTimeInstructionTreeWidget::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void RealTimeInstructionTreeWidget::AdjustTreeAppearance()
{
  m_selection_controller->SetDefaultExpandState();

  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
  static const bool adjust_columns_once = FixColumnAppearance(m_tree_view);
}

void RealTimeInstructionTreeWidget::OnTreeDoubleClick(const QModelIndex &index)
{
  if (index.column() == InstructionOperationViewModel::GetBreakpointColumn())
  {
    auto instruction = m_component_provider->GetSelected<InstructionItem>();
    emit ToggleBreakpointRequest(instruction);
  }
}

void RealTimeInstructionTreeWidget::OnCustomContextMenuRequested(const QPoint &pos)
{
  QMenu menu;

  // setting up menu section with standard collapse/expand block
  sup::gui::SetupCollapseExpandMenu(pos, menu, *m_tree_view);
  menu.addSeparator();

  // setting menu with "selective" expand option
  auto selective_expand_action = menu.addAction("Selective expand");
  selective_expand_action->setToolTip(
      "Expand all except instructions with property 'show as collapsed' set.");

  auto on_action = [this]() { m_selection_controller->SetDefaultExpandState(); };
  QObject::connect(selective_expand_action, &QAction::triggered, this, on_action);

  menu.exec(m_tree_view->mapToGlobal(pos));
}

void RealTimeInstructionTreeWidget::ScrollViewportToSelection()
{
  if (!m_viewport_follows_selection)
  {
    return;
  }

  ScrollTreeViewportToSelection(*m_tree_view);
}

}  // namespace sequencergui
