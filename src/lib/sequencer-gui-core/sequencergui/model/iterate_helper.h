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

#ifndef SEQUENCERGUI_MODEL_ITERATE_HELPER_H_
#define SEQUENCERGUI_MODEL_ITERATE_HELPER_H_

#include <functional>
#include <stack>

namespace sequencergui
{

/**
 * @brief Iterates in non-recursive manner over instruction tree and calls user callback.
 */

template <typename T>
void IterateInstruction(T item, std::function<void(T)> func)
{
  std::stack<T> stack;
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_ITERATE_HELPER_H_
