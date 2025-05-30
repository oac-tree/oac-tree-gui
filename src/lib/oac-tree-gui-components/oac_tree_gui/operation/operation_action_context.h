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

#ifndef OAC_TREE_GUI_OPERATION_OPERATION_ACTION_CONTEXT_H_
#define OAC_TREE_GUI_OPERATION_OPERATION_ACTION_CONTEXT_H_

#include <sup/gui/core/message_event.h>

#include <functional>
#include <optional>

namespace oac_tree_gui
{

class JobItem;
class RemoteConnectionService;
struct RemoteConnectionInfo;

/**
 * @brief The OperationActionContext struct contains callbacks necessary for OperationActionHandler
 * to function.
 */
struct OperationActionContext
{
  //! callback to get currently selected job
  std::function<JobItem*()> selected_job;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message;

  //!< callback to ask about remote job import information
  std::function<std::optional<RemoteConnectionInfo>()> get_remote_connection_info;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_OPERATION_ACTION_CONTEXT_H_
