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

#include "sequencergui/utils/sequencer_align_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/utils/align_node.h>
#include <sequencergui/utils/align_utils.h>

#include <mvvm/model/sessionmodel.h>

#include <QDebug>
#include <QPointF>
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
  return CreateAlignTree(container->GetInstructions());
}

std::unique_ptr<AlignNode> CreateAlignTree(std::vector<InstructionItem *> instructions)
{
  auto result = std::make_unique<AlignNode>();

  for (auto child : instructions)
  {
    result->Add(CreateAlignTree(child));
  }

  return result;
}

std::unique_ptr<AlignNode> CreateAlignTree(const InstructionItem *instruction)
{
  auto result = std::make_unique<AlignNode>();
  result->SetIdentifier(instruction->GetIdentifier());

  struct Data
  {
    const InstructionItem *instruction{nullptr};
    AlignNode *parent_node{nullptr};
  };

  std::stack<Data> node_stack;
  auto children = instruction->GetInstructions();
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

void TranslatePositions(const QPointF &reference, AlignNode &root_node)
{
  std::stack<AlignNode *> node_stack;
  node_stack.push(&root_node);

  double scale_x = GetAlignmentGridWidth() / root_node.GetNodeSize();
  auto translate_x = [x0 = root_node.GetX(), new_x0 = reference.x(), scale_x](auto x)
  { return (x - x0) * scale_x + new_x0; };

  double scale_y = GetAlignmentGridHeight() / root_node.GetNodeSize();
  auto translate_y = [y0 = root_node.GetY(), new_y0 = reference.y(), scale_y](auto y)
  { return (y - y0) * scale_y + new_y0; };

  while (!node_stack.empty())
  {
    auto node = node_stack.top();
    node_stack.pop();

    double x = node->GetX() * scale_x + reference.x();
    double y = node->GetY() * scale_x + reference.y();

    node->SetPos(translate_x(node->GetX()), translate_y(node->GetY()));

    auto children = node->GetChildren();
    // reverse iteration to get preorder
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      node_stack.push(*it);
    }
  }
}

void UpdatePositions(const AlignNode *node, InstructionContainerItem *container)
{
  UpdatePositions(node, container->GetInstructions());
}

void UpdatePositions(const AlignNode *node, std::vector<InstructionItem *> instructions)
{
  for (auto child : instructions)
  {
    UpdatePositions(node, child);
  }
}

void UpdatePositions(const AlignNode *node, InstructionItem *item)
{
  // Current implementation relies on the instruction identifier, that has been stored
  // in AlignNode on previous steps (while building AlignNodes from items). Identifier
  // is used to retrieve instruction items back from the model. Thus, InstructionItem must
  // be the part of some model.
  auto model = item->GetModel();

  if (!model)
  {
    throw std::runtime_error("Item must be the part of some model");
  }

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

void AlignInstructionTreeWalker(const QPointF &reference, InstructionItem *instruction)
{
  qDebug() << "Hello world";
  auto align_tree = CreateAlignTree(instruction);
  AlignNodes(*align_tree);
  TranslatePositions(reference, *align_tree);
  UpdatePositions(align_tree.get(), instruction);
}

void AlignInstructionTreeWalker(const QPointF &reference, InstructionContainerItem *container)
{
  auto align_tree = CreateAlignTree(container);
  AlignNodes(*align_tree);
  TranslatePositions(reference, *align_tree);
  UpdatePositions(align_tree.get(), container);
}

void AlignInstructionTreeWalker(const QPointF &reference,
                                std::vector<InstructionItem *> instructions)
{
  auto align_tree = CreateAlignTree(instructions);
  AlignNodes(*align_tree);
  TranslatePositions(reference, *align_tree);
  UpdatePositions(align_tree.get(), instructions);
}

}  // namespace sequencergui::algorithm
