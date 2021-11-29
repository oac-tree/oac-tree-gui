/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "graphicsscene.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/graphicsscenecontroller.h"
#include "sequencergui/nodeeditor/instructionlistwidget.h"
#include "sequencergui/nodeeditor/nodeconnection.h"
#include "sequencergui/nodeeditor/nodecontroller.h"
#include "sequencergui/nodeeditor/sceneutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

namespace
{
const double scene_origin_x{-3000.0};
const double scene_origin_y{0.0};

const QRectF default_scene_rect{QPointF{scene_origin_x, scene_origin_y}, QSizeF{6000, 6000}};

sequi::InstructionItem *GetInstruction(sequi::ConnectableView *view)
{
  return view ? const_cast<sequi::InstructionItem *>(view->connectableItem()->GetInstruction())
              : nullptr;
}

}  // namespace

namespace sequi
{
GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent), m_node_controller(new NodeController(this))
{
  setSceneRect(default_scene_rect);

  connect(m_node_controller.get(), &NodeController::connectionRequest, this,
          &GraphicsScene::onConnectionRequest);

  connect(m_node_controller.get(), &NodeController::selectionModeChangeRequest, this,
          &GraphicsScene::selectionModeChangeRequest);

  connect(this, &GraphicsScene::selectionChanged, this, &GraphicsScene::onSelectionChanged);
}

GraphicsScene::~GraphicsScene() = default;

void GraphicsScene::SetContext(SequencerModel *model, InstructionContainerItem *root_item)
{
  m_model = model;
  m_root_item = root_item;
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
    if (view->connectableItem()->GetInstruction() == instruction)
    {
      return view;
    }
  }
  return nullptr;
}

void GraphicsScene::onDeleteSelectedRequest()
{
  if (!m_model || !m_root_item)
  {
    throw std::runtime_error("Error in GraphicsScene: model not initialised");
  }

  // Break explicitely selected connections.
  for (auto connection : selectedViewItems<NodeConnection>())
  {
    disconnectConnectedViews(connection);
  }

  for (auto view : selectedViewItems<ConnectableView>())
  {
    // If the parent is intended to the deletion and has input connections, they have to be
    // disconnected first. This will prevent child item to be deleted when the parent is gone.
    for (auto connection : view->outputConnections())
    {
        disconnectConnectedViews(connection);
    }

    auto instruction = view->connectableItem()->GetInstruction();
    m_model->RemoveItem(instruction);
  }

}

void GraphicsScene::disconnectConnectedViews(NodeConnection *connection)
{
  // No actual view disconnection is going on here. We act on underlying ConnectableItem's.
  // All children items connected to the parents via this connection will be moved to the top of
  // the model (i.e. will become children of the model's root item). This in turn will trigger
  // corresponding ConnectableView recreation.

  auto instruction = GetInstruction(connection->childView());
  m_model->MoveItem(instruction, m_root_item, {"", -1});
  // No need to delete the connection explicitly. It will be done by ConnectableView via its
  // ports.
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat(InstructionListWidget::piecesMimeType()))
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

  auto mimeData = event->mimeData();
  if (!mimeData->hasFormat(InstructionListWidget::piecesMimeType()))
  {
    return;
  }

  auto requested_types =
      ModelView::Utils::GetStringList(mimeData->data(InstructionListWidget::piecesMimeType()));

  for (const auto &item_type : requested_types)
  {
    QPointF drop_pos(event->scenePos().x() - ref_view_rectangle.width() / 2,
                     event->scenePos().y() - ref_view_rectangle.height() / 2);

    if (auto item = dynamic_cast<InstructionItem *>(
            m_model->InsertNewItem(item_type.toStdString(), m_root_item));
        item)
    {
      item->SetX(drop_pos.x());
      item->SetY(drop_pos.y());
    }
  }
}

void GraphicsScene::onConnectionRequest(ConnectableView *childView, ConnectableView *parentView)
{
  auto child_instruction = GetInstruction(childView);
  auto parent_instruction = GetInstruction(parentView);
  m_model->MoveItem(child_instruction, parent_instruction, {"", -1});
}

void GraphicsScene::onSelectionChanged()
{
  auto selected = selectedViewItems<ConnectableView>();
  if (selected.empty())
  {
    return;
  }
  emit InstructionSelected(GetInstruction(selected.at(0)));
}

}  // namespace sequi
