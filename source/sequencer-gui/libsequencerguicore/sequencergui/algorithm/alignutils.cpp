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

//! Iterates over all nodes in a tree and build a contour map of `depth` .vs. `edge`.
//! `depth` is a position in the hierarchy (o - root node, 1 - direct children, etc)
//! `edge` is most left, or most right node coordinate at given level, depending on comparator
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

int GetMaxKey(const std::map<int, double>& map)
{
  return map.rbegin()->first;
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
  return GetCountour(node, mod_sum, [](auto x1, auto x2) { return std::min(x1, x2); });
}

std::map<int, double> GetRightCountour(AlignNode& node, double mod_sum)
{
  return GetCountour(node, mod_sum, [](auto x1, auto x2) { return std::max(x1, x2); });
}

void CalculateInitialX(AlignNode& node)
{
  for (auto child : node.GetChildren())
  {
    CalculateInitialX(*child);
  }

  // if no children
  if (node.IsLeaf())
  {
    // if there is a previous sibling in this set, set X to prevous sibling + designated distance
    if (!node.IsLeftMost())
    {
      node.SetX(node.GetPreviousSibling()->GetX() + node.GetNodeSize() + node.GetSiblingDistance());
    }
    else
    {
      // if this is the first node in a set, set X to 0
      node.SetX(0.0);
    }
  }
  // if there is only one child
  else if (node.GetSize() == 1)
  {
    // if this is the first node in a set, set it's X value equal to it's child's X value
    if (node.IsLeftMost())
    {
      node.SetX(node.GetChildren().at(0)->GetX());
    }
    else
    {
      node.SetX(node.GetPreviousSibling()->GetX() + node.GetNodeSize() + node.GetSiblingDistance());
      node.SetMod(node.GetX() - node.GetChildren().at(0)->GetX());
    }
  }
  else
  {
    auto leftChild = node.GetLeftMostChild();
    auto rightChild = node.GetRightMostChild();
    auto mid = (leftChild->GetX() + rightChild->GetX()) / 2;

    if (node.IsLeftMost())
    {
      node.SetX(mid);
    }
    else
    {
      node.SetX(node.GetPreviousSibling()->GetX() + node.GetNodeSize() + node.GetSiblingDistance());
      node.SetMod(node.GetX() - mid);
    }
  }

  //  if (node.Children.Count > 0 && !node.IsLeftMost())
  //  {
  //      // Since subtrees can overlap, check for conflicts and shift tree right if needed
  //      CheckForConflicts(node);
  //  }
}

void AlignNodes(AlignNode& node)
{
  InitializeNodes(node);
  CalculateInitialX(node);
}

void CheckForConflicts(AlignNode& node)
{
  const double treeDistance = 0.0;
  const double minDistance = treeDistance + node.GetNodeSize();
  double shiftValue = 0.0;

  auto nodeContour = GetLeftCountour(node, 0.0);
  auto sibling = node.GetLeftMostSibling();

  while (sibling != nullptr && sibling != &node)
  {
    auto siblingContour = GetRightCountour(*sibling, 0.0);

    for (int level = static_cast<int>(node.GetY()) + 1;
         level <= std::min(GetMaxKey(siblingContour), GetMaxKey(nodeContour)); level++)
    {
      double distance = nodeContour[level] - siblingContour[level];
      if (distance + shiftValue < minDistance)
      {
        shiftValue = minDistance - distance;
      }
    }

    if (shiftValue > 0)
    {
      node.SetX(node.GetX() + shiftValue);
      node.SetMod(node.GetMod() + shiftValue);

      CenterNodesBetween(node, *sibling);

      shiftValue = 0;
    }

    sibling = sibling->GetNextSibling();
  }
}

/*

        private static void CheckForConflicts(TreeNodeModel<T> node)
        {
            var minDistance = treeDistance + nodeSize;
            var shiftValue = 0F;

            var nodeContour = new Dictionary<int, float>();
            GetLeftContour(node, 0, ref nodeContour);

            var sibling = node.GetLeftMostSibling();
            while (sibling != null && sibling != node)
            {
                var siblingContour = new Dictionary<int, float>();
                GetRightContour(sibling, 0, ref siblingContour);

                for (int level = node.Y + 1; level <= Math.Min(siblingContour.Keys.Max(),
   nodeContour.Keys.Max()); level++)
                {
                    var distance = nodeContour[level] - siblingContour[level];
                    if (distance + shiftValue < minDistance)
                    {
                        shiftValue = minDistance - distance;
                    }
                }

                if (shiftValue > 0)
                {
                    node.X += shiftValue;
                    node.Mod += shiftValue;

                    CenterNodesBetween(node, sibling);

                    shiftValue = 0;
                }

                sibling = sibling.GetNextSibling();
            }
        }

  */

void CenterNodesBetween(AlignNode& leftNode, AlignNode& rightNode) {}

}  // namespace sequencergui::algorithm
