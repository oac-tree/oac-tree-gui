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

#ifndef OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_FACTORY_H_
#define OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_FACTORY_H_

#include <oac_tree_gui/jobsystem/i_automation_client.h>
#include <oac_tree_gui/jobsystem/remote_connection_info.h>

#include <functional>
#include <memory>

namespace oac_tree_gui
{

/**
 * Factory function to create automation clients for the give server info.
 */
using AutomationClientFunc =
    std::function<std::unique_ptr<IAutomationClient>(const AutomationServerInfo&)>;

/**
 * @brief Creates default function that will create automation clients.
 */
AutomationClientFunc GetAutomationClientFactoryFunc();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
