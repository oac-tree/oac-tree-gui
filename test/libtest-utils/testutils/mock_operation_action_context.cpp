/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mock_operation_action_context.h"

namespace oac_tree_gui::test
{

OperationActionContext MockOperationActionContext::CreateContext()
{
  OperationActionContext result;
  result.selected_job = [this]() { return OnSelectedJob(); };
  result.send_message = [this](const auto& message) { OnMessage(message); };
  result.get_remote_connection_info = [this]() { return OnGetRemoteConnectionInfo(); };
  return result;
}

}  // namespace oac_tree_gui::test
