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

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_MONITOR_HELPER_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_MONITOR_HELPER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

namespace sequencergui
{
class WorkspaceItem;
class DomainWorkspaceBuilder;
}  // namespace sequencergui

namespace sequencergui
{

//! Populate empty domain workspace with the content in WorkspaceItem.
//! If domain workspace is non-empty, will throw.
void PopulateDomainWorkspace(const WorkspaceItem& item, workspace_t& workspace);

}  // namespace sequencergui

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_MONITOR_HELPER_H_
