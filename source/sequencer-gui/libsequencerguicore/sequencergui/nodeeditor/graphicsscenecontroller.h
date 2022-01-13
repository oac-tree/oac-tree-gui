/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"

#include <memory>
#include <functional>

namespace sequencergui
{
class GraphicsScene;
class SequencerModel;
class InstructionContainerItem;

//! Listens for changes in SequencerModel and updates GraphicsScene.

class GraphicsSceneController : public ModelView::AbstractViewModelController
{
public:
  GraphicsSceneController(SequencerModel *model, GraphicsScene *graphics_scene);
  ~GraphicsSceneController() override;

  void SetAlignStrategy(std::function<void(InstructionContainerItem*)> strategy);

  void OnItemInserted(ModelView::SessionItem *parent,
                      const ModelView::TagIndex &tag_index) override;

  void OnAboutToRemoveItem(ModelView::SessionItem *parent,
                           const ModelView::TagIndex &tag_index) override;

  void OnDataChanged(ModelView::SessionItem *item, int role) override;

  void Init(InstructionContainerItem *root_item);

private:
  struct GraphicsSceneControllerImpl;
  std::unique_ptr<GraphicsSceneControllerImpl> p_impl;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENECONTROLLER_H
