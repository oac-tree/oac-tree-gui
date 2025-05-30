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

#include "node_graphics_scene.h"

#include "node_graphics_scene_action_handler.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/aggregate_factory.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/connectable_instruction_adapter.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/node_connection.h>
#include <oac_tree_gui/nodeeditor/node_connection_controller.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QGraphicsSceneDragDropEvent>
#include <QMessageBox>
#include <QMimeData>

namespace oac_tree_gui
{

namespace
{

QRectF GetDefaultSceneRect()
{
  return {oac_tree_gui::GetGraphicsViewportOrigin(), oac_tree_gui::GetGraphicsViewportSize()};
}

oac_tree_gui::InstructionItem *GetInstruction(oac_tree_gui::ConnectableView *view)
{
  return view ? const_cast<oac_tree_gui::InstructionItem *>(
                    view->GetConnectableItem()->GetInstruction())
              : nullptr;
}

//! Returns name encoded in the drop event.
std::string GetEncodedName(QGraphicsSceneDragDropEvent *event)
{
  return oac_tree_gui::GetNewInstructionType(event->mimeData());
}

//! Returns domain type from the drop event. If domain_type can't be deduced from the event data,
//! will return an empty string.
std::string GetRequestedDomainType(QGraphicsSceneDragDropEvent *event)
{
  return GetEncodedName(event);
}

}  // namespace

NodeGraphicsScene::NodeGraphicsScene(
    std::function<void(const sup::gui::MessageEvent &)> send_message_callback,
    QObject *parent_object)
    : QGraphicsScene(parent_object)
    , m_node_controller(new NodeConnectionController(this))
    , m_send_message_callback(send_message_callback)
    , m_action_handler(std::make_unique<NodeGraphicsSceneActionHandler>(CreateContext()))
{
  setSceneRect(GetDefaultSceneRect());

  connect(m_node_controller.get(), &NodeConnectionController::connectionRequest, this,
          &NodeGraphicsScene::onConnectionRequest);

  connect(m_node_controller.get(), &NodeConnectionController::OperationModeChangeRequest, this,
          &NodeGraphicsScene::OperationModeChangeRequest);

  // Strange bug in Qt6.3: if we use connection via lambda, as in code below, everyting works.
  // If we use classical connection &GraphicsScene::onSelectionChanged program crashes
  // with the following warning:

  //  ASSERT failure in oac_tree_gui::GraphicsScene: "Called object is not of the correct type
  //  (class destructor may have already run)", file /usr/include/qt6/QtCore/qobjectdefs_impl.h,
  //  line 155 Aborted (core dumped)

  connect(this, &NodeGraphicsScene::selectionChanged, this, [this]() { onSelectionChanged(); });
  //  connect(this, &GraphicsScene::selectionChanged, this, &GraphicsScene::onSelectionChanged); <--
  //  crashes in Qt6.3
}

NodeGraphicsScene::~NodeGraphicsScene() = default;

void NodeGraphicsScene::SetInstructionContainer(InstructionContainerItem *root_item)
{
  m_instruction_container = root_item;
}

//! Returns true if given scene is initialised (has model and instruction container assigned).

bool NodeGraphicsScene::HasContext()
{
  return GetModel() && m_instruction_container;
}

//! Resets context, removes and deletes objects on scene.

void NodeGraphicsScene::ResetContext()
{
  clear();
  m_instruction_container = nullptr;
}

std::vector<ConnectableView *> NodeGraphicsScene::GetConnectableViews()
{
  std::vector<ConnectableView *> result;
  for (auto &item : items())
  {
    if (auto child = dynamic_cast<ConnectableView *>(item); child)
    {
      if (child->parentItem() == nullptr)
      {
        result.push_back(child);
      }
    }
  }
  return result;
}

ConnectableView *NodeGraphicsScene::FindViewForInstruction(InstructionItem *instruction)
{
  auto views = GetConnectableViews();
  for (auto view : views)
  {
    if (view->GetConnectableItem()->GetInstruction() == instruction)
    {
      return view;
    }
  }
  return nullptr;
}

void NodeGraphicsScene::OnDeleteSelectedRequest()
{
  if (!HasContext())
  {
    qWarning("Error in GraphicsScene: context is not initialised");
    return;
  }

  mvvm::utils::BeginMacro(*GetModel(), "OnDeleteSelectedRequest");

  m_action_handler->RemoveInstruction();

  // Break remaining explicitely selected connections.
  auto selected_connections = GetSelectedViewItems<NodeConnection>();
  while (!selected_connections.empty())
  {
    disconnectConnectedViews(GetSelectedViewItems<NodeConnection>().at(0));

    // View disconnection will lead to the moving of an underlying item up under the root item.
    // This will lead to the view removal. As a result, selected connections might change the
    // status.
    selected_connections = GetSelectedViewItems<NodeConnection>();
  }

  mvvm::utils::EndMacro(*GetModel());
}

void NodeGraphicsScene::disconnectConnectedViews(NodeConnection *connection)
{
  // No actual view disconnection is going on here. We act on underlying ConnectableItem's.
  // All children items connected to the parents via this connection will be moved to the top of
  // the model (i.e. will become children of the model's root item). This in turn will trigger
  // corresponding ConnectableView recreation.

  auto instruction = GetInstruction(connection->childView());
  GetModel()->MoveItem(instruction, m_instruction_container, mvvm::TagIndex::Append());
  // No need to delete the connection explicitly. It will be done by ConnectableView via its
  // ports.
}

void NodeGraphicsScene::SetSelectedInstructions(const std::vector<InstructionItem *> &to_select)
{
  clearSelection();
  for (auto instruction : to_select)
  {
    if (auto view = FindViewForInstruction(instruction); view)
    {
      view->setSelected(true);
    }
  }
}

std::vector<InstructionItem *> NodeGraphicsScene::GetSelectedInstructions() const
{
  std::vector<InstructionItem *> result;
  for (const auto view : GetSelectedViewItems<ConnectableView>())
  {
    result.push_back(view->GetConnectableItem()->GetInstruction());
  }
  return result;
}

void NodeGraphicsScene::onConnectionRequest(ConnectableView *child_view,
                                            ConnectableView *parent_view)
{
  auto child_instruction = GetInstruction(child_view);
  auto parent_instruction = GetInstruction(parent_view);

  try
  {
    GetModel()->MoveItem(child_instruction, parent_instruction, mvvm::TagIndex::Append());
  }
  catch (const mvvm::MessageException &ex)
  {
    m_send_message_callback(sup::gui::CreateInvalidOperationMessage(ex.what()));
  }
}

void NodeGraphicsScene::DropInstructionTree(const std::string &name, const QPointF &ref_pos)
{
  m_action_handler->DropInstruction(name, {ref_pos.x(), ref_pos.y()});
}

void NodeGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat(kNewInstructionMimeType))
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

void NodeGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  if (!HasContext())
  {
    qWarning("Error in GraphicsScene: context is not initialised");
    QMessageBox::warning(nullptr, "Logic error", "Please create procedure first");
    return;
  }

  if (auto domain_type = GetRequestedDomainType(event); !domain_type.empty())
  {
    DropInstructionTree(domain_type, GetNodeDropPosition(event->scenePos()));
  }
}

void NodeGraphicsScene::onSelectionChanged()
{
  auto selected = GetSelectedViewItems<ConnectableView>();
  if (selected.empty())
  {
    return;
  }
  emit InstructionSelected(GetInstruction(selected.at(0)));
}

mvvm::ApplicationModel *NodeGraphicsScene::GetModel()
{
  return m_instruction_container
             ? dynamic_cast<mvvm::ApplicationModel *>(m_instruction_container->GetModel())
             : nullptr;
}

InstructionEditorContext NodeGraphicsScene::CreateContext()
{
  InstructionEditorContext result;
  result.instruction_container = [this]() { return m_instruction_container; };
  result.selected_instructions = [this]() { return GetSelectedInstructions(); };
  result.notify_request = [this](auto item)
  {
    const std::vector<InstructionItem *> to_select({dynamic_cast<InstructionItem *>(item)});
    SetSelectedInstructions(to_select);
  };

  result.create_instruction = [](const std::string &name) { return CreateInstructionTree(name); };

  result.send_message = m_send_message_callback;

  return result;
}

}  // namespace oac_tree_gui
