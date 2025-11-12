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

#ifndef OAC_TREE_GUI_JOBSYSTEM_REQUEST_TYPES_H_
#define OAC_TREE_GUI_JOBSYSTEM_REQUEST_TYPES_H_

#include <sup/dto/anyvalue.h>

#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The UserChoiceArgs struct represents information to give to the User while asking to make
 * a choice.
 */
struct UserChoiceArgs
{
  std::vector<std::string> options;
  sup::dto::AnyValue metadata;
};

/**
 * @brief The UserChoiceResult struct represent user choice.
 */
struct UserChoiceResult
{
  std::int32_t index{0};
  bool processed{false};
};

/**
 * @brief The UserInputArgs struct represents information to give to the User while asking for
 * input.
 */
struct UserInputArgs
{
  sup::dto::AnyValue value;
  std::string description;
};

/**
 * @brief The UserInputResult struct is a result of user input.
 */
struct UserInputResult
{
  sup::dto::AnyValue value;
  bool processed{false};
};

bool operator==(const UserChoiceArgs& lhs, const UserChoiceArgs& rhs);
bool operator!=(const UserChoiceArgs& lhs, const UserChoiceArgs& rhs);

bool operator==(const UserChoiceResult& lhs, const UserChoiceResult& rhs);
bool operator!=(const UserChoiceResult& lhs, const UserChoiceResult& rhs);

bool operator==(const UserInputArgs& lhs, const UserInputArgs& rhs);
bool operator!=(const UserInputArgs& lhs, const UserInputArgs& rhs);

bool operator==(const UserInputResult& lhs, const UserInputResult& rhs);
bool operator!=(const UserInputResult& lhs, const UserInputResult& rhs);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REQUEST_TYPES_H_
