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

#include "user_input_provider.h"

namespace oac_tree_gui
{

UserInputProvider::UserInputProvider(provider_callback_t callback) : m_request_queue(callback)
{
  connect(this, &UserInputProvider::InputRequest, this, &UserInputProvider::OnInputRequest,
          Qt::QueuedConnection);
}

UserInputResult UserInputProvider::GetUserInput(const UserInputArgs& args)
{
  auto queued_request_for_data = [this]() { emit InputRequest(); };
  return m_request_queue.GetData(args, queued_request_for_data);
}

//! Processes the request and sends the data to the waiting thread.
//! Method will be called by the GUI thread thanks to the queued connection.

void UserInputProvider::OnInputRequest()
{
  m_request_queue.OnDataRequest();
}

}  // namespace oac_tree_gui
