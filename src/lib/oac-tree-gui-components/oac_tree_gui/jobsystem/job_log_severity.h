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

#ifndef OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_SEVERITY_H_
#define OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_SEVERITY_H_

//! Provide log severity levels and accompanying utility functions.

#include <cstdint>
#include <string>

namespace oac_tree_gui
{

//! Defines severity levels for messages.

enum class Severity : std::uint8_t
{
  kEmergency = 0,
  kAlert,
  kCritical,
  kError,
  kWarning,
  kNotice,
  kInfo,
  kDebug,
  kTrace
};

std::string ToString(Severity severity);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_SEVERITY_H_
