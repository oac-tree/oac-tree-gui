/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/instructiontreewidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/widgets/topitemstreeview.h"

#include <QVBoxLayout>

namespace sequencergui
{

InstructionTreeWidget::InstructionTreeWidget(QWidget *parent)
    : QWidget(parent), m_tree_view(new mvvm::TopItemsTreeView)

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_tree_view);

  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          [this](auto) { emit InstructionSelected(GetSelectedInstruction()); });
}

void InstructionTreeWidget::SetProcedure(ProcedureItem *procedure)
{
  m_tree_view->SetItem(procedure ? procedure->GetInstructionContainer() : nullptr);
}

void InstructionTreeWidget::SetSelectedInstruction(InstructionItem *instruction)
{
  m_tree_view->SetSelectedItem(instruction);
}

void InstructionTreeWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
  m_tree_view->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionTreeWidget::GetSelectedInstructions() const
{
  return m_tree_view->GetSelectedItems<InstructionItem>();
}

InstructionItem *InstructionTreeWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

}  // namespace sequencergui
