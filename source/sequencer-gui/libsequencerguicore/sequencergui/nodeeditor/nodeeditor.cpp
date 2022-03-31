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

#include "sequencergui/nodeeditor/nodeeditor.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/graphicsscene.h"
#include "sequencergui/nodeeditor/graphicsscenecontroller.h"
#include "sequencergui/nodeeditor/graphicsview.h"
#include "sequencergui/nodeeditor/nodeeditortoolbar.h"
#include "sequencergui/nodeeditor/sceneutils.h"
#include "sequencergui/utils/sequenceralignutils.h"

#include <QDebug>
#include <QPointF>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

NodeEditor::NodeEditor(Qt::ToolBarArea area, QWidget *parent)
    : QMainWindow(parent)
    , m_tool_bar(new NodeEditorToolBar)
    , m_graphics_scene(new GraphicsScene)
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
{
  //  auto layout = new QVBoxLayout(this);
  //  layout->setContentsMargins(0, 0, 0, 0);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  //  layout->addWidget(m_tool_bar);
  //  layout->addWidget(m_graphics_view);

  m_tool_bar->setMovable(false);

  addToolBar(area, m_tool_bar);
  setCentralWidget(m_graphics_view);

  SetupConnections();
}

NodeEditor::~NodeEditor() = default;

void NodeEditor::SetModel(SequencerModel *model)
{
  m_model = model;
}

void NodeEditor::SetProcedure(ProcedureItem *procedure)
{
  if (procedure)
  {
    auto instruction_container = procedure->GetInstructionContainer();
    m_graphics_scene->SetContext(m_model, instruction_container);
    m_scene_controller = std::make_unique<GraphicsSceneController>(m_model, m_graphics_scene);

    auto scene_rect = m_graphics_scene->sceneRect();
    const QPointF reference_point = m_graphics_scene->sceneRect().center();
    auto align_strategy = [reference_point](auto container)
    { AlignTree(reference_point, container); };
    m_scene_controller->SetAlignStrategy(align_strategy);

    m_scene_controller->Init(instruction_container);

    m_graphics_view->onCenterView();
  }
}

std::vector<InstructionItem *> NodeEditor::GetSelectedInstructions() const
{
  return m_graphics_scene->GetSelectedInstructions();
}

void NodeEditor::SetSelectedInstructions(const std::vector<InstructionItem *> &instructions) const
{
  m_graphics_scene->SetSelectedInstructions(instructions);
}

void NodeEditor::SetupConnections()
{
  // Propagates delete request from the graphics view to the scene.
  connect(m_graphics_view, &GraphicsView::deleteSelectedRequest, m_graphics_scene,
          &GraphicsScene::OnDeleteSelectedRequest);

  // Forward instruction selection from graphics scene
  connect(m_graphics_scene, &GraphicsScene::InstructionSelected, this,
          &NodeEditor::InstructionSelected);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &GraphicsView::selectionModeChanged, m_tool_bar,
          &NodeEditorToolBar::onViewSelectionMode);

  // Propagate selection mode change from toolbar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::selectionMode, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Center view from toolBar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::centerView, m_graphics_view, &GraphicsView::onCenterView);

  // Propagate zoom request from a toolbar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::changeScale, m_graphics_view,
          &GraphicsView::onChangeScale);

  // Propagate selection request from GraphicsScene to GraphicsView
  connect(m_graphics_scene, &GraphicsScene::selectionModeChangeRequest, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Propagate align request from toolBar to GraphicsScene. All children of currently selected
  // parent will be aligned.
  auto on_align = [this]()
  {
    auto selected = m_graphics_scene->GetSelectedViewItems<ConnectableView>();
    for (auto view : selected)
    {
      auto item = view->GetConnectableItem()->GetInstruction();
      AlignInstructionTree(view->pos(), item, /*force*/ true);
    }
  };
  connect(m_tool_bar, &NodeEditorToolBar::alignSelectedRequest, this, on_align);

  auto on_align_v2 = [this]()
  {
    auto selected = m_graphics_scene->GetSelectedViewItems<ConnectableView>();
    if (selected.empty())
    {
      return;
    }

    if (selected.size() == 1)
    {
      auto view = selected.front();
      auto item = view->GetConnectableItem()->GetInstruction();
      algorithm::AlignInstructionTreeWalker(view->pos(), item);
    }
    else
    {
      QRectF rect = selected.front()->sceneBoundingRect();
      std::vector<InstructionItem *> items;
      for (auto view : selected)
      {
        rect = rect.united(view->sceneBoundingRect());
        items.push_back(view->GetConnectableItem()->GetInstruction());
      }
      algorithm::AlignInstructionTreeWalker(rect.center(), items);
    }
  };
  connect(m_tool_bar, &NodeEditorToolBar::alignSelectedRequestV2, this, on_align_v2);
}

}  // namespace sequencergui
