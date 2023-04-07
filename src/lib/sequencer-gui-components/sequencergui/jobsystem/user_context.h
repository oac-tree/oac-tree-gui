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

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_

#include <QStringList>
#include <functional>

#include <sequencergui/jobsystem/request_types.h>

namespace sequencergui
{

//! Aggregates necessary information to communicate with the user.
//!
struct UserContext
{
  //!< A callback to returns user input as a text.
  //! Initial vaue, and the description are provided as callback parameters.
  using userinput_callback_t = std::function<UserInputResult(UserInputArgs)>;
  userinput_callback_t m_user_input_callback;

  //!< A callback to returns user choice as an integer.
  //! Initial vaues, and the description are provided as callback parameters.
  using userchoice_callback_t = std::function<UserChoiceResult(const UserChoiceArgs&)>;
  userchoice_callback_t m_user_choice_callback;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_CONTEXT_H_
