/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/usercontroller.h"

namespace sequi
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

//! Set given value as used input.
void UserController::SetAsUserInput(const std::string &value)
{
  m_input_data.update_top(value); // will release waiting in GetUserInput
}

void UserController::SetupConnections()
{
  // Connects input request (issued from the runner thread), with GUI thread (queued connection)
  connect(this, &UserController::InputRequest, this, &UserController::OnInputRequest,
          Qt::QueuedConnection);
}

}  // namespace sequi
