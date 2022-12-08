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

#include "suppvmonitor/workspace_event.h"

namespace suppvmonitor
{

bool WorkspaceEvent::operator==(const WorkspaceEvent& other) const
{
  return m_variable_name == other.m_variable_name && m_value == other.m_value;
}

bool WorkspaceEvent::operator!=(const WorkspaceEvent& other) const
{
  return !(*this == other);
}

}  // namespace suppvmonitor
