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

#ifndef OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENT_HELPER_H_
#define OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENT_HELPER_H_

//! @file
//! Contains a collection of helper methods for domain_events.

#include <oac_tree_gui/jobsystem/domain_events.h>

#include <string>

namespace oac_tree_gui
{

/**
 * @brief The DomainEventToStringVisitor is a visitor to print debug information for domain events.
 */
struct DomainEventToStringVisitor
{
  std::string operator()(const std::monostate &event) const;

  std::string operator()(const ::oac_tree_gui::InstructionStateUpdatedEvent &event) const;

  std::string operator()(const ::oac_tree_gui::VariableUpdatedEvent &event) const;

  std::string operator()(const ::oac_tree_gui::JobStateChangedEvent &event) const;

  std::string operator()(const ::oac_tree_gui::LogEvent &event) const;

  std::string operator()(const ::oac_tree_gui::ActiveInstructionChangedEvent &event) const;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENTS_H_
