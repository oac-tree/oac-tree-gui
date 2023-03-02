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

#include "anyvalue_item_utils.h"

#include "anyvalue_item.h"

#include <algorithm>
#include <stack>
#include <stdexcept>

namespace sup::gui
{

void UpdateAnyValueItemScalarData(const AnyValueItem &source, AnyValueItem &target)
{
  if (!source.IsScalar() || !target.IsScalar())
  {
    throw std::logic_error("Item(s) are not scalars");
  }

  if (source.GetAnyTypeName() != target.GetAnyTypeName())
  {
    throw std::logic_error("Item types do not match");
  }

  target.SetData(source.Data());
}

void UpdateAnyValueItemData(const AnyValueItem &source, AnyValueItem &target)
{
  struct Node
  {
    const AnyValueItem *source{nullptr};
    AnyValueItem *target{nullptr};
  };

  std::stack<Node> nodes;
  nodes.push({&source, &target});

  while (!nodes.empty())
  {
    auto &node = nodes.top();

    if (node.source->IsScalar())
    {
      UpdateAnyValueItemScalarData(*node.source, *node.target);
      nodes.pop();
    }
    else
    {
      auto source_children = node.source->GetChildren();
      auto target_children = node.target->GetChildren();
      nodes.pop();

      if (source_children.size() != target_children.size())
      {
        throw std::logic_error("Number of children do not match");
      }

      // preparing vector containing pairs of children
      std::vector<Node> new_children;
      auto on_transform = [&nodes](const AnyValueItem *source, AnyValueItem *target) {
        return Node{source, target};
      };
      std::transform(source_children.begin(), source_children.end(), target_children.begin(),
                     std::back_inserter(new_children), on_transform);

      // adding to the stack in reverse order
      for (auto it = new_children.rbegin(); it != new_children.rend(); ++it)
      {
        nodes.push(*it);
      }
    }
  }
}

}  // namespace sup::gui
