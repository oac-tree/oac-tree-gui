/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUPPVMONITOR_WORKSPACE_EVENT_H_
#define SUPPVMONITOR_WORKSPACE_EVENT_H_

#include <sup/dto/anyvalue.h>

namespace suppvmonitor
{

struct WorkspaceEvent
{
  std::string m_variable_name;
  sup::dto::AnyValue m_value;
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_WORKSPACE_EVENT_H_
