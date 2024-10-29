/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_

#include <sequencergui/jobsystem/request_types.h>

#include <functional>

namespace sequencergui
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_
