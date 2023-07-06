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

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/viewmodel/instruction_viewmodel.h>
#include <sequencergui/widgets/style_utils.h>
#include <sequencergui/operation/realtime_tree_view.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/widgets/widget_utils.h>

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
  tree->header()->setSectionResizeMode(2, QHeaderView::Stretch);
  tree->header()->setSectionResizeMode(3, QHeaderView::Fixed);
  tree->setColumnWidth(3, mvvm::utils::UnitSize(1));
  return true;
}

}  // namespace

namespace sequencergui
{

RealTimeInstructionTreeWidget::RealTimeInstructionTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new RealTimeTreeView)
    , m_component_provider(mvvm::CreateProvider<InstructionViewModel>(m_tree_view))
    , m_custom_header(new sup::gui::CustomHeaderView(this))
{
  setWindowTitle("InstructionTree");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);

  auto on_click = [this](auto index)
  {
    auto item = m_component_provider->GetSelected<InstructionItem>();
    emit InstructionClicked(item);
  };
  connect(m_tree_view, &QTreeView::clicked, this, on_click);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view);

  ReadSettings();
}

RealTimeInstructionTreeWidget::~RealTimeInstructionTreeWidget()
{
  WriteSettings();
}

void RealTimeInstructionTreeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_component_provider->SetItem(procedure_item ? procedure_item->GetInstructionContainer()
                                               : nullptr);
  if (procedure_item)
  {
    AdjustTreeAppearance();
  }
}

void RealTimeInstructionTreeWidget::SetSelectedInstruction(InstructionItem *item)
{
  m_component_provider->SetSelectedItem(item);
}

void RealTimeInstructionTreeWidget::SetSelectedInstructions(std::vector<InstructionItem *> items)
{
  m_component_provider->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(items));
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
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
  m_tree_view->expandAll();
  static const bool adjust_columns_once = FixColumnAppearance(m_tree_view);
}

}  // namespace sequencergui
