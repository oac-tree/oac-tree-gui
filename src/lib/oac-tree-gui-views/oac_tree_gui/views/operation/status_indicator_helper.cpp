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

#include "status_indicator_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <map>

namespace oac_tree_gui
{

BusySignType GetBusySignTypeForInstructionStatus(InstructionStatus status)
{
  static const std::map<InstructionStatus, BusySignType> status_to_indicator_type{
      {InstructionStatus::kNotStarted, BusySignType::kHidden},
      {InstructionStatus::kNotFinished, BusySignType::kAnimated},
      {InstructionStatus::kRunning, BusySignType::kAnimated},
      {InstructionStatus::kSuccess, BusySignType::kHidden},
      {InstructionStatus::kFailure, BusySignType::kHidden},
      {InstructionStatus::kHalted, BusySignType::kHidden},
      {InstructionStatus::kUndefined, BusySignType::kHidden}};

  auto it = status_to_indicator_type.find(status);
  if (it == status_to_indicator_type.end())
  {
    throw RuntimeException("Unknown instruction status");
  }
  return it->second;
}

QColor GetColorForInstructionStatus(InstructionStatus status)
{
  // We use the same color for not started and not finished states.
  static const std::map<InstructionStatus, QColor> status_to_color{
      {InstructionStatus::kNotStarted, Qt::lightGray},
      {InstructionStatus::kNotFinished, Qt::yellow},
      {InstructionStatus::kRunning, Qt::yellow},
      {InstructionStatus::kSuccess, Qt::green},
      {InstructionStatus::kFailure, Qt::red},
      {InstructionStatus::kHalted, Qt::cyan},
      {InstructionStatus::kUndefined, Qt::darkGray}};
  auto it = status_to_color.find(status);
  if (it == status_to_color.end())
  {
    throw RuntimeException("Unexpected instruction status: " + ToString(status));
  }

  return it->second;
}

}  // namespace oac_tree_gui
