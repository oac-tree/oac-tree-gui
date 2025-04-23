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

// ----------------------------------------------------------------------------
// UserChoiceArgs
// ----------------------------------------------------------------------------

bool UserChoiceArgs::operator==(const UserChoiceArgs& other) const
{
  return (options == other.options) && (metadata == other.metadata);
}

bool UserChoiceArgs::operator!=(const UserChoiceArgs& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// UserChoiceResult
// ----------------------------------------------------------------------------

bool UserChoiceResult::operator==(const UserChoiceResult& other) const
{
  return (index == other.index) && (processed == other.processed);
}

bool UserChoiceResult::operator!=(const UserChoiceResult& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// UserInputArgs
// ----------------------------------------------------------------------------

bool UserInputArgs::operator==(const UserInputArgs& other) const
{
  return value == other.value && description == other.description;
}

bool UserInputArgs::operator!=(const UserInputArgs& other) const
{
  return !(*this == other);
}

// ----------------------------------------------------------------------------
// UserInputArgs
// ----------------------------------------------------------------------------

bool UserInputResult::operator==(const UserInputResult& other) const
{
  return (value == other.value) && (processed == other.processed);
}

bool UserInputResult::operator!=(const UserInputResult& other) const
{
  return !(*this == other);
}

}  // namespace oac_tree_gui
