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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_
#define SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_

#include <mvvm/viewmodel/abstract_viewmodel_controller.h>

#include <functional>
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

class GraphicsSceneController : public mvvm::AbstractViewModelController
{
public:
  GraphicsSceneController(mvvm::SessionModelInterface *model, GraphicsScene *graphics_scene);
  ~GraphicsSceneController() override;

  void SetAlignStrategy(std::function<void(InstructionContainerItem *)> strategy);

  void OnModelEvent(const mvvm::ItemInsertedEvent &event) override;

  void OnModelEvent(const mvvm::AboutToRemoveItemEvent &event) override;

  void OnModelEvent(const mvvm::DataChangedEvent &event) override;

  void Init(InstructionContainerItem *root_item);

private:
  struct GraphicsSceneControllerImpl;
  std::unique_ptr<GraphicsSceneControllerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_CONTROLLER_H_
