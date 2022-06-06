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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENECONTROLLER_H
#define SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENECONTROLLER_H

#include "mvvm/viewmodel/abstract_viewmodel_controller.h"

#include <functional>
#include <memory>

namespace mvvm
{
class ApplicationModel;
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
  GraphicsSceneController(mvvm::ApplicationModel *model, GraphicsScene *graphics_scene);
  ~GraphicsSceneController() override;

  void SetAlignStrategy(std::function<void(InstructionContainerItem *)> strategy);

  void OnItemInserted(mvvm::SessionItem *parent, const mvvm::TagIndex &tag_index) override;

  void OnAboutToRemoveItem(mvvm::SessionItem *parent, const mvvm::TagIndex &tag_index) override;

  void OnDataChanged(mvvm::SessionItem *item, int role) override;

  void Init(InstructionContainerItem *root_item);

private:
  struct GraphicsSceneControllerImpl;
  std::unique_ptr<GraphicsSceneControllerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENECONTROLLER_H
