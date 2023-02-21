/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_EVENT_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_EVENT_H_

#include <sup/dto/anyvalue.h>

namespace suppvmonitor
{

struct WorkspaceEvent
{
  std::string variable_name;
  sup::dto::AnyValue value;
  bool is_available{false};

  bool operator==(const WorkspaceEvent& other) const;
  bool operator!=(const WorkspaceEvent& other) const;
};

}  // namespace suppvmonitor

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_EVENT_H_
