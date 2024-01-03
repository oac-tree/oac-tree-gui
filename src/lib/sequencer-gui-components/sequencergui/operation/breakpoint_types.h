/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_OPERATION_BREAKPOINT_TYPES_H_
#define SEQUENCERGUI_OPERATION_BREAKPOINT_TYPES_H_

#include <mvvm/model/path.h>

namespace sequencergui
{

enum class BreakpointStatus
{
  kNotSet,
  kSet,
  kDisabled
};

struct BreakpointInfo
{
  BreakpointStatus status;
  mvvm::Path path;  //! path to the instruction item
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_TYPES_H_
