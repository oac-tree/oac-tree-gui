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

#include <stack>

namespace sup::gui
{

void UpdateAnyValueItemData(const AnyValueItem &target, AnyValueItem &source)
{
  struct Node
  {
    const AnyValueItem* target{nullptr};
    const AnyValueItem* source{nullptr};
  };

  std::stack<Node> nodes;
  nodes.push({&target, &source});

  while (!nodes.empty())
  {
    auto& node = nodes.top();

  }



}

}  // namespace sup::gui
