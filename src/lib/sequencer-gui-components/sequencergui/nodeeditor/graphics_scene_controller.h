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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_
#define SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_

#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{
class SessionModelInterface;
}

namespace sequencergui
{
class GraphicsScene;
class SequencerModel;
class InstructionContainerItem;

//! Listens for changes in SequencerModel and updates GraphicsScene.

class GraphicsSceneController
{
public:
  GraphicsSceneController(mvvm::SessionModelInterface *model, GraphicsScene *graphics_scene);
  ~GraphicsSceneController();

  void OnModelEvent(const mvvm::ItemInsertedEvent &event);

  void OnModelEvent(const mvvm::AboutToRemoveItemEvent &event);

  void OnModelEvent(const mvvm::DataChangedEvent &event);

  void Init(InstructionContainerItem *root_item);

private:
  struct GraphicsSceneControllerImpl;
  std::unique_ptr<GraphicsSceneControllerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_
