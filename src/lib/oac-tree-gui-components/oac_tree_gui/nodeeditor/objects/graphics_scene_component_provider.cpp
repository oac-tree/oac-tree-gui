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
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/connectable_shape_factory.h>
#include <oac_tree_gui/nodeeditor/graphics_scene_action_handler.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/connectable_view_model_controller.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_connection_guide.h>
#include <mvvm/nodeeditor/node_connection_shape.h>

namespace oac_tree_gui
{

GraphicsSceneComponentProvider::GraphicsSceneComponentProvider(
    std::function<void(const sup::gui::MessageEvent &)> send_message_callback,
    std::function<std::string(const std::string &)> object_to_plugin_name, QGraphicsScene *scene,
    mvvm::SessionItem *instruction_container)
    : QObject(nullptr)
    , m_send_message_callback(send_message_callback)
    , m_object_to_plugin_name(object_to_plugin_name)
    , m_scene(scene)
    , m_instruction_container(instruction_container)
    , m_connection_guide(std::make_unique<mvvm::NodeConnectionGuide>(m_scene))
    , m_viewmodel_controller(CreateViewModelController())
    , m_graphics_scene_action_handler(CreateSceneActionHandler())
    , m_instruction_editor_action_handler(CreateInstructionEditorActionHandler())
{
  SetupConnections();
}

GraphicsSceneComponentProvider::~GraphicsSceneComponentProvider() = default;

void GraphicsSceneComponentProvider::OnDeleteSelected()
{
  mvvm::utils::BeginMacro(*GetModel(), "GraphicsSceneComponentProvider::OnDeleteSelected");

  m_instruction_editor_action_handler->RemoveInstruction();

  auto selected_connections = mvvm::GetSelectedShapes<mvvm::NodeConnectionShape>(*m_scene);
  while (!selected_connections.empty())
  {
    m_graphics_scene_action_handler->Disconnect(selected_connections.front()->GetNodeConnection());
    selected_connections = mvvm::GetSelectedShapes<mvvm::NodeConnectionShape>(*m_scene);
  }

  mvvm::utils::EndMacro(*GetModel());
}

std::vector<InstructionItem *> GraphicsSceneComponentProvider::GetSelectedInstructions() const
{
  std::vector<InstructionItem *> result;
  for (auto shape : mvvm::GetSelectedShapes<mvvm::ConnectableShape>(*m_scene))
  {
    result.push_back(mvvm::GetUnderlyingItem<InstructionItem>(shape));
  }
  return result;
}

void GraphicsSceneComponentProvider::SetSelectedInstructions(
    const std::vector<InstructionItem *> &to_select)
{
  m_scene->clearSelection();
  for (auto instruction : to_select)
  {
    if (auto view = m_viewmodel_controller->FindShapeForItem(*instruction); view)
    {
      view->setSelected(true);
    }
  }
}

void GraphicsSceneComponentProvider::DropInstruction(const std::string &item_type,
                                                     const position_t &pos)
{
  m_instruction_editor_action_handler->DropInstruction(item_type, pos);
}

mvvm::ISessionModel *GraphicsSceneComponentProvider::GetModel() const
{
  return m_instruction_container ? m_instruction_container->GetModel() : nullptr;
}

void GraphicsSceneComponentProvider::SetupConnections()
{
  connect(m_scene, &QGraphicsScene::selectionChanged, this,
          &GraphicsSceneComponentProvider::OnSceneSelectionChanged);

  connect(m_connection_guide.get(), &mvvm::NodeConnectionGuide::connectionRequest, this,
          [this](auto port_start, auto port_end)
          { m_graphics_scene_action_handler->ConnectPorts(port_start, port_end); });

  connect(m_connection_guide.get(), &mvvm::NodeConnectionGuide::connectionStarted, this,
          &GraphicsSceneComponentProvider::connectionStarted);
  connect(m_connection_guide.get(), &mvvm::NodeConnectionGuide::connectionFinished, this,
          &GraphicsSceneComponentProvider::connectionFinished);
}

void GraphicsSceneComponentProvider::OnSceneSelectionChanged()
{
  emit selectionChanged();
}

std::unique_ptr<mvvm::ConnectableViewModelController>
GraphicsSceneComponentProvider::CreateViewModelController()
{
  auto factory = std::make_unique<ConnectableShapeFactory>();
  auto result = std::make_unique<mvvm::ConnectableViewModelController>(std::move(factory), m_scene);
  result->InitScene(m_instruction_container);
  return result;
}

std::unique_ptr<IGraphicsSceneActionHandler>
GraphicsSceneComponentProvider::CreateSceneActionHandler()
{
  return std::make_unique<GraphicsSceneActionHandler>(m_instruction_container,
                                                      m_send_message_callback);
}

InstructionEditorContext GraphicsSceneComponentProvider::CreateContext()
{
  InstructionEditorContext result;
  result.instruction_container = [this]() { return m_instruction_container; };
  result.selected_instructions = [this]() { return GetSelectedInstructions(); };
  result.notify_request = [this](auto item)
  {
    const std::vector<InstructionItem *> to_select({dynamic_cast<InstructionItem *>(item)});
    SetSelectedInstructions(to_select);
  };

  // result.create_instruction = [](const std::string &name) { return CreateInstructionTree(name);
  // };

  result.send_message = m_send_message_callback;
  result.object_to_plugin_name = m_object_to_plugin_name;
  return result;
}

std::unique_ptr<IInstructionEditorActionHandler>
GraphicsSceneComponentProvider::CreateInstructionEditorActionHandler()
{
  return std::make_unique<InstructionEditorActionHandler>(CreateContext());
}

}  // namespace oac_tree_gui
