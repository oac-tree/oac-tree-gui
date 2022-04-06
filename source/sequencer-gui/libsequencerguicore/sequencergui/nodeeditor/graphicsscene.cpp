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

#include "graphicsscene.h"

#include "sequencergui/composer/instructionlistwidget.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/graphicsscenecontroller.h"
#include "sequencergui/nodeeditor/nodeconnection.h"
#include "sequencergui/nodeeditor/nodecontroller.h"
#include "sequencergui/nodeeditor/sceneutils.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/widgets/widgetutils.h"

#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMessageBox>
#include <QMimeData>
#include <sstream>

namespace
{
const double scene_origin_x{-3000.0};
const double scene_origin_y{0.0};

const QRectF default_scene_rect{QPointF{scene_origin_x, scene_origin_y}, QSizeF{6000, 6000}};

sequencergui::InstructionItem *GetInstruction(sequencergui::ConnectableView *view)
{
  return view ? const_cast<sequencergui::InstructionItem *>(
             view->GetConnectableItem()->GetInstruction())
              : nullptr;
}

}  // namespace

namespace sequencergui
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

//! Returns true if given scene is initialised (has model and instruction container assigned).

bool GraphicsScene::HasContext()
{
  return m_model && m_root_item;
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

void GraphicsScene::onConnectionRequest(ConnectableView *childView, ConnectableView *parentView)
{
  auto child_instruction = GetInstruction(childView);
  auto parent_instruction = GetInstruction(parentView);

  try
  {
    m_model->MoveItem(child_instruction, parent_instruction, {"", -1});
  }
  catch (const mvvm::InvalidMoveException &ex)
  {
    std::ostringstream ostr;
    ostr << "Can't connect instructions. Maximum number of children exceeded? "
         << "Details: '" << ex.what() << "'";
    // FIXME implement unit test after implementing Error report mechanism
    qWarning("%s", ostr.str().c_str());
  }
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
  if (!HasContext())
  {
    qWarning("Error in GraphicsScene: context is not initialised");
    QMessageBox::warning(nullptr, "Logic error", "Please create procedure first");
    return;
  }

  // for later coordinate calculation, where to drop
  static QRectF ref_view_rectangle = ConnectableViewRectangle();

  auto mimeData = event->mimeData();
  if (!mimeData->hasFormat(InstructionListWidget::piecesMimeType()))
  {
    return;
  }

  auto requested_types =
      mvvm::utils::GetStringList(mimeData->data(InstructionListWidget::piecesMimeType()));

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

void GraphicsScene::onSelectionChanged()
{
  auto selected = GetSelectedViewItems<ConnectableView>();
  if (selected.empty())
  {
    return;
  }
  emit InstructionSelected(GetInstruction(selected.at(0)));
}

}  // namespace sequencergui
