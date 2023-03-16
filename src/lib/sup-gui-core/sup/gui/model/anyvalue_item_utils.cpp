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

#include <sup/gui/core/exceptions.h>

#include <algorithm>
#include <stack>

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
        throw RuntimeException(
            "While updating target AnyValue from source the different layout "
            "of target and source has been detected. The number of children "
            "does not match");
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

bool IsSuitableScalarType(const AnyValueArrayItem &array, const std::string &scalar_type)
{
  if (array.GetChildren().size() == 0)
  {
    return true;
  }

  if (auto scalar = array.GetItem<AnyValueScalarItem>(""); scalar)
  {
    if (scalar->GetAnyTypeName() == scalar_type)
    {
      return true;
    }
  }

  return false;
}

}  // namespace sup::gui
