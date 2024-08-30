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

#ifndef SEQUENCERGUI_COMPONENTS_APP_TYPES_H_
#define SEQUENCERGUI_COMPONENTS_APP_TYPES_H_

//! @file
//! Common types for application widgets.

namespace sequencergui
{

enum class WorkspacePresentationType
{
  kWorkspaceTree,  //!< full tree with workspace variables
  kWorkspaceTable  //!< experimental table with workspace variables
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_APP_TYPES_H_
