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

#include "user_controller.h"

#include <mvvm/widgets/widget_utils.h>

namespace sequencergui
{

UserController::UserController(QObject *parent) : QObject(parent)
{
  SetupConnections();
}

void UserController::SetUserContext(const UserContext &context)
{
  m_user_context = context;
}

//! Returns user input as a string.
//! This call is blocking and is expected to be fired from the runner thread.

std::string UserController::GetUserInput(const std::string &current_value,
                                         const std::string &description)
{
  // queued connection
  emit InputRequest(QString::fromStdString(current_value), QString::fromStdString(description));

  auto result = m_input_data.wait_and_pop();  // wait for the result
  return *result;
}

//! Method to request the user one of the given option.

int UserController::GetUserChoice(const std::vector<std::string> &choices,
                                  const std::string &description)
{
  // queued connection
  emit ChoiceRequest(mvvm::utils::GetStringList(choices), QString::fromStdString(description));

  auto result = m_choice_data.wait_and_pop();  // wait for the result
  return *result;
}

//! Set given value as used input.
void UserController::SetAsUserInput(const std::string &value)
{
  m_input_data.update_top(value);  // will release waiting in GetUserInput
}

void UserController::SetAsUserChoice(int value)
{
  m_choice_data.update_top(value);  // will release waiting in GetUserChoice
}

//! Processes user input. Expected to be called from the GUI thread
void UserController::OnInputRequest(const QString &current_value, const QString &description)
{
  if (m_user_context.m_user_input_callback)
  {
    SetAsUserInput(m_user_context.m_user_input_callback(current_value, description).toStdString());
  }
  else
  {
    // This is an abnormal case when the user input is required from the runner thread, but a
    // callback to ask the user wasn't set. Seems that throwing is not the right choice here, since
    // the call was done in queued connection and nobody knows where throwing will end up.
    // std::terminate() would cure everything, but we opt for more sneaky solution.

    // Let's provide the runner with some input and see if it can swallow it.
    SetAsUserInput("No user input was provided");
  }
}

//! Processes user choice. Expected to be called from the GUI thread
void UserController::OnUserChoiceRequest(const QStringList &choices, const QString &description)
{
  if (m_user_context.m_user_choice_callback)
  {
    SetAsUserChoice(m_user_context.m_user_choice_callback(choices, description));
  }
  else
  {
    // This is an abnormal case when the user input is required from the runner thread, but a
    // callback to ask the user wasn't set. Seems that throwing is not the right choice here, since
    // the call was done in queued connection and nobody knows where throwing will end up.
    // std::terminate() would cure everything, but we opt for more sneaky solution.

    // Let's provide the runner with some input and see if it can swallow it.
    SetAsUserInput("No user input was provided");
  }
}

//! Connects requests issued from the runner thread, with the GUI thread.
void UserController::SetupConnections() const
{
  // Connects input request.
  connect(this, &UserController::InputRequest, this, &UserController::OnInputRequest,
          Qt::QueuedConnection);

  // Connects choice request.
  connect(this, &UserController::ChoiceRequest, this, &UserController::OnUserChoiceRequest,
          Qt::QueuedConnection);
}

}  // namespace sequencergui
