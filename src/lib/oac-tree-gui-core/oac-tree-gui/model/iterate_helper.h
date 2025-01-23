/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_ITERATE_HELPER_H_
#define SEQUENCERGUI_MODEL_ITERATE_HELPER_H_

#include <functional>
#include <stack>

namespace sequencergui
{

/**
 * @brief Iterates in non-recursive manner over instruction tree and calls user callback.
 */
template <typename ItemT>
void IterateInstruction(ItemT item, std::function<void(ItemT)> func)
{
  std::stack<ItemT> stack;
  stack.push(item);

  while (!stack.empty())
  {
    auto item = stack.top();
    stack.pop();

    func(item);

    auto children = item->GetInstructions();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
}

/**
 * @brief Iterates over all instructions and their children in given container and calls user
 * callback.
 */
template <typename ItemT, typename ContainerT>
void IterateInstructionContainer(const ContainerT& container, std::function<void(ItemT)> func)
{
  for (ItemT instruction : container)
  {
    IterateInstruction<ItemT>(instruction, func);
  }
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_ITERATE_HELPER_H_
