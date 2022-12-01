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

#include "graphics_scene.h"

#include "sequencergui/nodeeditor/graphics_scene_controller.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/widgets/widget_utils.h>
#include <sequencergui/core/message_handler_interface.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/aggregate_factory.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/node_connection.h>
#include <sequencergui/nodeeditor/node_controller.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/utils/sequencer_align_utils.h>
#include <sequencergui/widgets/item_list_widget.h>

#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMessageBox>
#include <QMimeData>
#include <sstream>

namespace
{

QRectF GetDefaultSceneRect()
{
  return {sequencergui::GetGraphicsViewportOrigin(), sequencergui::GetGraphicsViewportSize()};
}

sequencergui::InstructionItem *GetInstruction(sequencergui::ConnectableView *view)
{
  return view ? const_cast<sequencergui::InstructionItem *>(
             view->GetConnectableItem()->GetInstruction())
              : nullptr;
}

//! Returns name encoded in the drop event.
std::string GetEncodedName(QGraphicsSceneDragDropEvent *event)
{
  auto event_data = event->mimeData();
  if (event_data->hasFormat(sequencergui::ItemListWidget::piecesMimeType()))
  {
    auto binary_data = event_data->data(sequencergui::ItemListWidget::piecesMimeType());
    auto list = mvvm::utils::GetStringList(binary_data);
    return list.empty() ? std::string() : list.front().toStdString();
  }

  return {};
}

//! Returns domain type from the drop event. If domain_type can't be deduced from the event data,
//! will return an empty string.
std::string GetRequestedDomainType(QGraphicsSceneDragDropEvent *event)
{
  return GetEncodedName(event);
}

bool IsAggregateName(const std::string &name)
{
  ::sequencergui::AggregateFactory factory;
  return factory.Contains(name);
}

}  // namespace

namespace sequencergui
{
GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent), m_node_controller(new NodeController(this))
{
  setSceneRect(GetDefaultSceneRect());

  connect(m_node_controller.get(), &NodeController::connectionRequest, this,
          &GraphicsScene::onConnectionRequest);

  connect(m_node_controller.get(), &NodeController::selectionModeChangeRequest, this,
          &GraphicsScene::selectionModeChangeRequest);

  // Strange bug in Qt6.3: if we use connection via lambda, as in code below, everyting works.
  // If we use classical connection &GraphicsScene::onSelectionChanged program crashes
  // with the following warning:

  //  ASSERT failure in sequencergui::GraphicsScene: "Called object is not of the correct type
  //  (class destructor may have already run)", file /usr/include/qt6/QtCore/qobjectdefs_impl.h,
  //  line 155 Aborted (core dumped)

  connect(this, &GraphicsScene::selectionChanged, this, [this]() { onSelectionChanged(); });
  //  connect(this, &GraphicsScene::selectionChanged, this, &GraphicsScene::onSelectionChanged); <--
  //  crashes in Qt6.3
}

GraphicsScene::~GraphicsScene() = default;

void GraphicsScene::SetInstructionContainer(InstructionContainerItem *root_item)
{
  m_root_item = root_item;
}

void GraphicsScene::SetMessageHandler(std::unique_ptr<MessageHandlerInterface> message_handler)
{
  m_message_handler = std::move(message_handler);
}

//! Returns true if given scene is initialised (has model and instruction container assigned).

bool GraphicsScene::HasContext()
{
  return GetModel() && m_root_item;
}

//! Resets context, removes and deletes objects on scene.

void GraphicsScene::ResetContext()
{
  clear();
  m_root_item = nullptr;
}

std::vector<ConnectableView *> GraphicsScene::GetConnectableViews()
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

ConnectableView *GraphicsScene::FindViewForInstruction(InstructionItem *instruction)
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

void GraphicsScene::OnDeleteSelectedRequest()
{
  if (!HasContext())
  {
    qWarning("Error in GraphicsScene: context is not initialised");
    return;
  }

  // Break explicitely selected connections.
  for (auto connection : GetSelectedViewItems<NodeConnection>())
  {
    disconnectConnectedViews(connection);
  }

  for (auto view : GetSelectedViewItems<ConnectableView>())
  {
    // If the parent is intended to the deletion and has input connections, they have to be
    // disconnected first. This will prevent child item to be deleted when the parent is gone.
    for (auto connection : view->GetOutputConnections())
    {
      disconnectConnectedViews(connection);
    }

    auto instruction = view->GetConnectableItem()->GetInstruction();
    GetModel()->RemoveItem(instruction);
  }
}

void GraphicsScene::disconnectConnectedViews(NodeConnection *connection)
{
  // No actual view disconnection is going on here. We act on underlying ConnectableItem's.
  // All children items connected to the parents via this connection will be moved to the top of
  // the model (i.e. will become children of the model's root item). This in turn will trigger
  // corresponding ConnectableView recreation.

  auto instruction = GetInstruction(connection->childView());
  GetModel()->MoveItem(instruction, m_root_item, {"", -1});
  // No need to delete the connection explicitly. It will be done by ConnectableView via its
  // ports.
}

void GraphicsScene::SetSelectedInstructions(const std::vector<InstructionItem *> &to_select)
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

std::vector<InstructionItem *> GraphicsScene::GetSelectedInstructions() const
{
  std::vector<InstructionItem *> result;
  for (const auto view : GetSelectedViewItems<ConnectableView>())
  {
    result.push_back(view->GetConnectableItem()->GetInstruction());
  }
  return result;
}

void GraphicsScene::onConnectionRequest(ConnectableView *child_view, ConnectableView *parent_view)
{
  auto child_instruction = GetInstruction(child_view);
  auto parent_instruction = GetInstruction(parent_view);

  try
  {
    GetModel()->MoveItem(child_instruction, parent_instruction, {"", -1});
  }
  catch (const mvvm::MessageException &ex)
  {
    if (m_message_handler)
    {
      std::ostringstream ostr;
      ostr << ex.what();
      m_message_handler->SendMessage(ostr.str());
    }
    else
    {
      throw;
    }
  }
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat(ItemListWidget::piecesMimeType()))
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  // for later coordinate calculation, where to drop
  static QRectF ref_view_rectangle = ConnectableViewRectangle();

  if (!HasContext())
  {
    qWarning("Error in GraphicsScene: context is not initialised");
    QMessageBox::warning(nullptr, "Logic error", "Please create procedure first");
    return;
  }

  QPointF drop_pos(event->scenePos().x() - ref_view_rectangle.width() / 2,
                   event->scenePos().y() - ref_view_rectangle.height() / 2);

  if (auto domain_type = GetRequestedDomainType(event); !domain_type.empty())
  {
    // FIXME refactor the mess
    if (IsAggregateName(domain_type))
    {
      // FIXME remove cast
      auto item =
          AddAggregate(dynamic_cast<SequencerModel *>(GetModel()), m_root_item, domain_type);
      algorithm::AlignInstructionTreeWalker(drop_pos, item);
    }
    else
    {
      // FIXME remove cast
      auto item = AddSingleInstruction(dynamic_cast<SequencerModel *>(GetModel()), m_root_item,
                                       domain_type);
      item->SetX(drop_pos.x());
      item->SetY(drop_pos.y());
    }
  }
}

void GraphicsScene::onSelectionChanged()
{
  auto selected = GetSelectedViewItems<ConnectableView>();
  if (selected.empty())
  {
    return;
  }
  emit InstructionSelected(GetInstruction(selected.at(0)));
}

mvvm::ApplicationModel *GraphicsScene::GetModel()
{
  return m_root_item ? dynamic_cast<mvvm::ApplicationModel *>(m_root_item->GetModel()) : nullptr;
}

}  // namespace sequencergui
