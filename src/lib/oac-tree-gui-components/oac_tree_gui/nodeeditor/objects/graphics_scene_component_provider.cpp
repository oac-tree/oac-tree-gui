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

#include "graphics_scene_component_provider.h"

#include <oac_tree_gui/composer/instruction_editor_action_handler.h>
#include <oac_tree_gui/nodeeditor/connectable_shape_factory.h>
#include <oac_tree_gui/nodeeditor/graphics_scene_action_handler.h>

#include <mvvm/nodeeditor/connectable_view_model_controller.h>
#include <mvvm/nodeeditor/node_connection_guide.h>

namespace oac_tree_gui
{

GraphicsSceneComponentProvider::GraphicsSceneComponentProvider(
    std::function<void(const sup::gui::MessageEvent &)> send_message_callback,
    QGraphicsScene *scene, mvvm::SessionItem *node_container)
    : QObject(nullptr)
    , m_send_message_callback(send_message_callback)
    , m_scene(scene)
    , m_node_container(node_container)
    , m_connection_guide(std::make_unique<mvvm::NodeConnectionGuide>(m_scene))
    , m_viewmodel_controller(CreateViewModelController())
    , m_graphics_scene_action_handler(CreateSceneActionHandler())
    , m_instruction_editor_action_handler(CreateInstructionEditorActionHandler())
{
}

GraphicsSceneComponentProvider::~GraphicsSceneComponentProvider() = default;

std::unique_ptr<mvvm::ConnectableViewModelController>
GraphicsSceneComponentProvider::CreateViewModelController()
{
  auto factory = std::make_unique<ConnectableShapeFactory>();
  auto result = std::make_unique<mvvm::ConnectableViewModelController>(std::move(factory), m_scene);
  result->InitScene(m_node_container);
  return result;
}

std::unique_ptr<IGraphicsSceneActionHandler>
GraphicsSceneComponentProvider::CreateSceneActionHandler()
{
  return std::make_unique<GraphicsSceneActionHandler>(m_node_container, m_send_message_callback);
}

InstructionEditorContext GraphicsSceneComponentProvider::CreateContext()
{
  return {};
}

std::unique_ptr<IInstructionEditorActionHandler>
GraphicsSceneComponentProvider::CreateInstructionEditorActionHandler()
{
  return std::make_unique<InstructionEditorActionHandler>(CreateContext());
}

}  // namespace oac_tree_gui
