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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_

//! @file
//! Collection of various utility functions for job execution.

#include <string>
#include <vector>

namespace oac_tree_gui
{

//! Returns reg-exp pattern for vector with labels.
//! example: {"INFO", "DEBUG"} -> "(INFO|DEBUG)"
std::string GetRegExpPattern(const std::vector<std::string>& data);

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
