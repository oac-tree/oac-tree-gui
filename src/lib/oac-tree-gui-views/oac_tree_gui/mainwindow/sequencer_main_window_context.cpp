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

#include "sequencer_main_window_context.h"

#include <sup/gui/app/default_command_service.h>

namespace oac_tree_gui
{

SequencerMainWindowContext::SequencerMainWindowContext()
    : m_command_service(sup::gui::CreateDefaultCommandService())
{
}

SequencerMainWindowContext::~SequencerMainWindowContext() = default;

sup::gui::IAppCommandService &SequencerMainWindowContext::GetCommandService()
{
  return *m_command_service;
}

}  // namespace oac_tree_gui
