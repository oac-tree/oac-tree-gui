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

#ifndef OAC_TREE_GUI_OPERATION_OPERATION_ACTION_HELPER_H_
#define OAC_TREE_GUI_OPERATION_OPERATION_ACTION_HELPER_H_

//! @file
//! Collection of stand-alone helper functions to handle operation actions, i.e. for import remote
//! and local jobs.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <functional>
#include <memory>

namespace oac_tree_gui
{

class IRemoteConnectionService;
class JobItem;
class IJobHandler;
struct UserContext;

/**
 * @brief Creates domain procedure from the given JobItem.
 *
 * Will throw if creation is not possible.
 */
std::unique_ptr<procedure_t> CreateDomainProcedure(const JobItem& item);

/**
 * @brief Creates factory function which is capable of creating job handlers, when JobItem is given
 * to it.
 *
 * JobItem can be one of: local, imported, or remote.
 *
 * Usage:
 * auto factory_func = GetJobHandlerFactoryFunc(context, service);
 * handler = factory_func(job_item);
 *
 * @param user_context The user interaction context, which will be used to create
 * AnstractJobHandler.
 * @param service Remote connection service used to create remote job handler.
 */
std::function<std::unique_ptr<IJobHandler>(JobItem&)> GetJobHandlerFactoryFunc(
    const UserContext& user_context, IRemoteConnectionService& service);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_OPERATION_ACTION_HELPER_H_
