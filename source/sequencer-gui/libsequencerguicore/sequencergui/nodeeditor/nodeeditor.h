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

#ifndef SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H
#define SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H

#include <QMainWindow>

class QKeyEvent;
class QToolBar;

namespace mvvm
{
class ApplicationModel;
}

namespace sequencergui
{
class GraphicsView;
class GraphicsScene;
class GraphicsSceneController;
class InstructionItem;
class NodeEditorToolBar;
class ProcedureItem;
class MessageHandlerInterface;

//! NodeEditor widget (central part of SequencerComposerView).

class NodeEditor : public QMainWindow
{
  Q_OBJECT

public:
  explicit NodeEditor(Qt::ToolBarArea area = Qt::TopToolBarArea, QWidget* parent = nullptr);
  ~NodeEditor() override;

  void SetModel(mvvm::ApplicationModel* model);

  void SetProcedure(ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions) const;

  std::unique_ptr<MessageHandlerInterface> CreateMessageHandler();

signals:
  void InstructionSelected(sequencergui::InstructionItem* selected);

private:
  void SetupConnections();

  NodeEditorToolBar* m_tool_bar{nullptr};
  GraphicsScene* m_graphics_scene{nullptr};
  GraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<GraphicsSceneController> m_scene_controller;
  std::unique_ptr<MessageHandlerInterface> m_graphics_view_message_handler;

  mvvm::ApplicationModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H
