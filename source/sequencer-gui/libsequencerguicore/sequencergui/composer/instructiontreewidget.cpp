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

namespace
{
// FIXME move to mvvm:: and remove duplication in composerprocedureeditor.cpp
template <typename T>
std::vector<T *> CastedItems(const std::vector<const mvvm::SessionItem *> &items)
{
  std::vector<T *> result;
  for (auto item : items)
  {
    if (auto casted_item = dynamic_cast<const T *>(item); casted_item)
    {
      result.push_back(const_cast<T *>(casted_item));
    }
  }

  return result;
}

}  // namespace

namespace sequencergui
{

InstructionTreeWidget::InstructionTreeWidget(QWidget *parent)
    : QWidget(parent), m_instruction_tree(new mvvm::TopItemsTreeView)

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_instruction_tree);
}

void InstructionTreeWidget::SetProcedure(ProcedureItem *procedure)
{
  m_instruction_tree->SetItem(procedure ? procedure->GetInstructionContainer() : nullptr);
}

void InstructionTreeWidget::SetSelectedInstruction(InstructionItem *instruction)
{
  m_instruction_tree->SetSelectedItem(instruction);
}

void InstructionTreeWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
//  std::vector<mvvm::SessionItem *> items;
//  std::copy(instructions.begin(), instructions.end(), std::back_inserter(items));
//  m_instruction_tree->SetSelectedItems(items);
  m_instruction_tree->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionTreeWidget::GetSelectedInstructions() const
{
  return m_instruction_tree->GetSelectedItems<InstructionItem>();
}

InstructionItem *InstructionTreeWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

}  // namespace sequencergui
