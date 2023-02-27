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

#include <sup/gui/dto/anyvalue_item.h>

#include <stack>
#include <stdexcept>

namespace sup::gui
{

void UpdateAnyValueItemScalarData(const AnyValueItem &source, AnyValueItem &target)
{
  if (!source.IsScalar() || ! target.IsScalar())
  {
    throw std::logic_error("Item(s) are not scalars");
  }

  if (source.GetAnyTypeName() != target.GetAnyTypeName())
  {
    throw std::logic_error("Item tpes do not match");
  }

  target.SetData(source.Data());
}

void UpdateAnyValueItemData(const AnyValueItem &source, AnyValueItem &target)
{
  struct Node
  {
    const AnyValueItem *target{nullptr};
    const AnyValueItem *source{nullptr};
  };

  std::stack<Node> nodes;
  nodes.push({&target, &source});

  while (!nodes.empty())
  {
    auto &node = nodes.top();
  }
}

}  // namespace sup::gui
