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

namespace
{
template <typename T>
std::map<int, double> GetCountour(sequencergui::algorithm::AlignNode& node, double mod_sum,
                                  T comparator)
{
  std::map<int, double> result;

  struct Data
  {
    sequencergui::algorithm::AlignNode* node{nullptr};
    double mod_sum{0.0};
  };

  std::stack<Data> node_stack;

  node_stack.push({&node, mod_sum});

  while (!node_stack.empty())
  {
    auto [node, mod_sum] = node_stack.top();
    node_stack.pop();

    int level = static_cast<int>(node->GetY());
    if (auto it = result.find(level); it == result.end())
    {
      result.insert({level, node->GetX() + mod_sum});
    }
    else
    {
      it->second = comparator(it->second, node->GetX() + mod_sum);
    }

    auto children = node->GetChildren();
    // reverse iteration to get preorder
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      node_stack.push({*it, mod_sum + node->GetMod()});
    }
  }

  return result;
}

}  // namespace

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
  return GetCountour(node, mod_sum, [](auto x, auto y) { return std::min(x, y); });
}

std::map<int, double> GetRightCountour(AlignNode& node, double mod_sum)
{
  return GetCountour(node, mod_sum, [](auto x, auto y) { return std::max(x, y); });
}

}  // namespace sequencergui::algorithm
