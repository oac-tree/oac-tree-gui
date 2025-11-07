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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "request_types.h"

namespace oac_tree_gui
{

bool operator==(const UserChoiceArgs& lhs, const UserChoiceArgs& rhs)
{
  return (lhs.options == rhs.options) && (lhs.metadata == rhs.metadata);
}

bool operator!=(const UserChoiceArgs& lhs, const UserChoiceArgs& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const UserChoiceResult& lhs, const UserChoiceResult& rhs)
{
  return (lhs.index == rhs.index) && (lhs.processed == rhs.processed);
}

bool operator!=(const UserChoiceResult& lhs, const UserChoiceResult& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const UserInputArgs& lhs, const UserInputArgs& rhs)
{
  return lhs.value == rhs.value && lhs.description == rhs.description;
}

bool operator!=(const UserInputArgs& lhs, const UserInputArgs& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const UserInputResult& lhs, const UserInputResult& rhs)
{
  return (lhs.value == rhs.value) && (lhs.processed == rhs.processed);
}

bool operator!=(const UserInputResult& lhs, const UserInputResult& rhs)
{
  return !(lhs == rhs);
}

}  // namespace oac_tree_gui
