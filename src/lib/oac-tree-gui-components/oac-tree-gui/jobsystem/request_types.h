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

  bool operator==(const UserChoiceArgs& other) const;
  bool operator!=(const UserChoiceArgs& other) const;
};

/**
 * @brief The UserChoiceResult struct represent user choice.
 */
struct UserChoiceResult
{
  int index{0};
  bool processed{false};

  bool operator==(const UserChoiceResult& other) const;
  bool operator!=(const UserChoiceResult& other) const;
};

/**
 * @brief The UserInputArgs struct represents information to give to the User while asking for
 * input.
 */
struct UserInputArgs
{
  sup::dto::AnyValue value;
  std::string description;

  bool operator==(const UserInputArgs& other) const;
  bool operator!=(const UserInputArgs& other) const;
};

/**
 * @brief The UserInputResult struct is a result of user input.
 */
struct UserInputResult
{
  sup::dto::AnyValue value;
  bool processed{false};

  bool operator==(const UserInputResult& other) const;
  bool operator!=(const UserInputResult& other) const;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REQUEST_TYPES_H_
