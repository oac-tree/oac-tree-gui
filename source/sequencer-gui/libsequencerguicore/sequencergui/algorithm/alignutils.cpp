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

#include "sequencergui/algorithm/alignutils.h"

#include "sequencergui/algorithm/alignnode.h"

#include "mvvm/utils/containerutils.h"

#include <algorithm>
#include <stack>

namespace sequencergui::algorithm
{

// Visits tree in preorder and sets initial values to the node parameters.
void InitializeNodes(AlignNode& node)
{
  struct Data
  {
    AlignNode* node{nullptr};
    double depth{0.0};
  };

  std::stack<Data> node_stack;

  node_stack.push({&node, 0.0});

  while (!node_stack.empty())
  {
    auto [node, depth] = node_stack.top();
    node_stack.pop();

    node->SetX(-1.0);
    node->SetY(depth);
    node->SetMod(0.0);

    auto children = node->GetChildren();
    // reverse iteration to get preorder
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      node_stack.push({*it, depth + 1.0});
    }
  }
}

std::map<int, double> GetLeftCountour(AlignNode& node, double mod_sum)
{
  std::map<int, double> result;
  GetLeftCountour(node, mod_sum, result);
  return result;
}

void GetLeftCountour(AlignNode& node, double mod_sum, std::map<int, double>& values)
{
  int level = static_cast<int>(node.GetY());
  if (auto it = values.find(level); it == values.end())
  {
    values.insert({level, node.GetX() + mod_sum});
  }
  else
  {
    it->second = std::min(it->second, node.GetX() + mod_sum);
  }

  mod_sum += node.GetMod();
  for (auto child : node.GetChildren())
  {
    GetLeftCountour(*child, mod_sum, values);
  }
}

}  // namespace sequencergui::algorithm
