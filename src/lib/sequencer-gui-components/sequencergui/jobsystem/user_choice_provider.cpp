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

#include "user_choice_provider.h"

namespace sequencergui
{

UserChoiceProvider::UserChoiceProvider(provider_callback_t callback) : m_provider_callback(callback)
{
  connect(this, &UserChoiceProvider::ChoiceRequest, this, &UserChoiceProvider::OnChoiceRequest,
          Qt::QueuedConnection);
}

UserChoiceResult UserChoiceProvider::GetUserChoice(const UserChoiceArgs& args)
{
  RequestData request_data;

  {
    std::unique_lock<std::mutex> lock(m_mutex);
    request_data.args = args;
    m_stack.push(&request_data);
  }

  emit ChoiceRequest();  // queued connection

  return request_data.request_handler.GetData();
}

//! Processes user choice and send the data to the waiting thread.
//! Method will be called by the GUI thread thanks to the queued connection.

void UserChoiceProvider::OnChoiceRequest()
{
  RequestData* request_data{nullptr};

  {
    std::unique_lock<std::mutex> lock(m_mutex);
    assert(!m_stack.empty());

    request_data = m_stack.front();
    m_stack.pop();
  }

  auto args = request_data->args;
  request_data->request_handler.SendData(m_provider_callback(args));
}

}  // namespace sequencergui
