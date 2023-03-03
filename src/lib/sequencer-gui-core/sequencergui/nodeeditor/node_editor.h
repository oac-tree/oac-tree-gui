/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_H_
#define SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_H_

#include "sup/gui/components/message_handler_interface.h"

#include <QWidget>

class QKeyEvent;
class QToolBar;

namespace sup::gui
{
class MessageHandlerInterface;
}

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

//! NodeEditor widget (central part of SequencerComposerView).

class NodeEditor : public QWidget
{
  Q_OBJECT

public:
  explicit NodeEditor(QWidget* parent = nullptr);
  ~NodeEditor() override;

  void SetProcedure(ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions) const;

  std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandler();

  std::unique_ptr<QToolBar> CreateToolBar();

signals:
  void InstructionSelected(sequencergui::InstructionItem* selected);

private:
  void SetupConnections();

  GraphicsScene* m_graphics_scene{nullptr};
  GraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<GraphicsSceneController> m_scene_controller;
  std::unique_ptr<sup::gui::MessageHandlerInterface> m_graphics_view_message_handler;

  ProcedureItem* m_procedure_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_H_
