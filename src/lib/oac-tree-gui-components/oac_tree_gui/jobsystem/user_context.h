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

#ifndef OAC_TREE_GUI_JOBSYSTEM_USER_CONTEXT_H_
#define OAC_TREE_GUI_JOBSYSTEM_USER_CONTEXT_H_

#include <oac_tree_gui/jobsystem/request_types.h>

#include <functional>

namespace oac_tree_gui
{

/**
 * @brief The UserContext class aggregates necessary information to communicate with the user.
 */
struct UserContext
{
  //!< a callback to return user input
  std::function<UserInputResult(const UserInputArgs&)> user_input_callback;

  //!< a callback to return user choice
  std::function<UserChoiceResult(const UserChoiceArgs&)> user_choice_callback;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_USER_CONTEXT_H_
