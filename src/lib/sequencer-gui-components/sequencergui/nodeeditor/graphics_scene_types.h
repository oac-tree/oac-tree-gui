/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_TYPES_H_
#define SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_TYPES_H_

namespace sequencergui
{
enum ESelectionModes
{
  kSimpleSelection,
  kRubberSelection,
  kHandDrag,
  kUnknownSelection
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_TYPES_H_
