/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_NODEEDITOR_NODE_EDITOR_H_
#define SEQUENCERGUI_VIEWS_NODEEDITOR_NODE_EDITOR_H_

#include <sup/gui/core/message_handler_interface.h>

#include <QWidget>

class QKeyEvent;

namespace sup::gui
{
class MessageHandlerInterface;
class VisibilityAgentBase;
}  // namespace sup::gui

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
class NodeEditorActions;
class ProcedureItem;

//! NodeEditor widget (central part of SequencerComposerView).

class NodeEditor : public QWidget
{
  Q_OBJECT

public:
  explicit NodeEditor(QWidget* parent_widget = nullptr);
  ~NodeEditor() override;

  void SetProcedure(ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions) const;

  std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandler();

signals:
  void InstructionSelected(sequencergui::InstructionItem* selected);

private:
  void SetupConnections();
  void OnAlignRequest();
  void SetupController();

  NodeEditorActions* m_actions{nullptr};
  GraphicsScene* m_graphics_scene{nullptr};
  GraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<GraphicsSceneController> m_scene_controller;
  std::unique_ptr<sup::gui::MessageHandlerInterface> m_graphics_view_message_handler;
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};

  ProcedureItem* m_procedure_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_NODEEDITOR_NODE_EDITOR_H_
