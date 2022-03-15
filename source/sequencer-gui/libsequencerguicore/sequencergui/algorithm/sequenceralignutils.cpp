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

#include "sequencergui/algorithm/sequenceralignutils.h"

#include "sequencergui/algorithm/alignnode.h"
#include "sequencergui/algorithm/alignutils.h"
#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/sequenceritems.h"

#include <QDebug>
#include <stack>
#include <stdexcept>

namespace
{
sequencergui::algorithm::AlignNode *PopulateNode(const sequencergui::InstructionItem &item,
                                                 sequencergui::algorithm::AlignNode &node)
{
  auto child = node.Add<sequencergui::algorithm::AlignNode>();
  child->SetIdentifier(item.GetIdentifier());
  return child;
}

}  // namespace

namespace sequencergui::algorithm
{

std::unique_ptr<AlignNode> CreateAlignTree(const InstructionContainerItem *container)
{
  auto result = std::make_unique<AlignNode>();

  struct Data
  {
    InstructionItem *instruction{nullptr};
    AlignNode *parent_node{nullptr};
  };

  std::stack<Data> node_stack;

  auto children = container->GetInstructions();
  for (auto it = children.rbegin(); it != children.rend(); ++it)
  {
    node_stack.push({*it, result.get()});
  }

  while (!node_stack.empty())
  {
    auto [instruction, node] = node_stack.top();
    node_stack.pop();

    auto new_parent = PopulateNode(*instruction, *node);

    auto children = instruction->GetInstructions();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      node_stack.push({*it, new_parent});
    }
  }

  return result;
}

void UpdatePositions(const AlignNode *node, InstructionContainerItem *container)
{
  auto model = container->GetModel();

  std::stack<const AlignNode *> node_stack;
  node_stack.push(node);

  while (!node_stack.empty())
  {
    auto node = node_stack.top();
    node_stack.pop();

    // instructions are found using identifier stored on board of node
    if (auto instruction = dynamic_cast<InstructionItem *>(model->FindItem(node->GetIdentifier()));
        instruction)
    {
      instruction->SetX(node->GetX());
      instruction->SetY(node->GetY());
    }

    auto children = node->GetChildren();
    // reverse iteration to get preorder
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      node_stack.push(*it);
    }
  }
}

void AlignInstructionTreeWalker(const QPointF &reference, InstructionItem *instruction, bool force)
{
  qDebug() << "Hello world";
}

void AlignInstructionTreeWalker(const QPointF &reference, InstructionContainerItem *container,
                                bool force)
{
  auto align_tree = CreateAlignTree(container);
  AlignNodes(*align_tree);
  UpdatePositions(align_tree.get(), container);
}

}  // namespace sequencergui::algorithm
