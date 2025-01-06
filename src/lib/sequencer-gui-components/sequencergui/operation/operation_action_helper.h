/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_OPERATION_OPERATION_ACTION_HELPER_H_
#define SEQUENCERGUI_OPERATION_OPERATION_ACTION_HELPER_H_

//! @file
//! Collection of stand-alone helper functions to handle operation actions, i.e. for import remote
//! and local jobs.

#include <functional>
#include <memory>

namespace sequencergui
{

class IRemoteConnectionService;
class JobItem;
class IJobHandler;
struct UserContext;

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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_ACTION_HELPER_H_
