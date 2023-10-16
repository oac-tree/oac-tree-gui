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

#include "standard_children_strategies.h"

#include <sequencergui/model/sequencer_item_helper.h>

#include <mvvm/model/sessionitem.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace
{

/**
 * @brief Returns copy of container, without elements in the exclude list.
 */

template <typename T, typename F>
T FilterElements(const T &container, const F &to_exclude)
{
  T result;

  auto not_in_exclude_list = [&to_exclude](const auto &x)
  { return !mvvm::utils::Contains(to_exclude, x); };

  std::copy_if(container.begin(), container.end(), std::back_inserter(result), not_in_exclude_list);

  return result;
}

}  // namespace

namespace sequencergui
{

std::vector<mvvm::SessionItem *> VariableChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  if (!item)
  {
    return {};
  }

  const std::vector<mvvm::SessionItem *> to_exclude = {GetNameItem(*item),
                                                       GetIsAvailableItem(*item)};

  auto result = FilterElements(item->GetAllItems(), to_exclude);

  return result;
}

}  // namespace sequencergui
