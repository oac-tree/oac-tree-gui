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

#include "monitor_realtime_tree_widget.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/viewmodel/instruction_viewmodel.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/item_view_component_provider.h>

#include <sup/gui/widgets/custom_header_view.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorRealTimeTreeWidget::MonitorRealTimeTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<InstructionViewModel>(m_tree_view))
    , m_custom_header(new sup::gui::CustomHeaderView(this))
{
  setWindowTitle("InstructionTree");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_tree_view->setHeader(m_custom_header);

  auto on_click = [this](auto index)
  {
    // FIXME refactor this hell
    auto item = const_cast<mvvm::SessionItem *>(
        m_component_provider->GetViewModel()->GetSessionItemFromIndex(index));
    emit InstructionClicked(dynamic_cast<InstructionItem *>(item));
  };
  connect(m_tree_view, &QTreeView::clicked, this, on_click);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view);
}

MonitorRealTimeTreeWidget::~MonitorRealTimeTreeWidget() = default;

void MonitorRealTimeTreeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_component_provider->SetItem(procedure_item ? procedure_item->GetInstructionContainer()
                                               : nullptr);
  m_tree_view->expandAll();
  AdjustColumnWidth();
}

void MonitorRealTimeTreeWidget::SetSelectedInstruction(InstructionItem *item)
{
  m_component_provider->SetSelectedItem(item);
}

void MonitorRealTimeTreeWidget::AdjustColumnWidth()
{
  if (m_custom_header->IsAdjustedByUser())
  {
    m_custom_header->RestoreSize();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
}

}  // namespace sequencergui
