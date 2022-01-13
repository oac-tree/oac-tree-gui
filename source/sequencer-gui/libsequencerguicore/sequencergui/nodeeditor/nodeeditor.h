/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

namespace sequencergui
{
class GraphicsView;
class GraphicsScene;
class SequencerModel;
class GraphicsSceneController;
class InstructionItem;
class NodeEditorToolBar;
class ProcedureItem;

//! NodeEditor widget (central part of SequencerComposerView).

class NodeEditor : public QMainWindow
{
  Q_OBJECT

public:
  NodeEditor(Qt::ToolBarArea area = Qt::TopToolBarArea, QWidget* parent = nullptr);
  ~NodeEditor();

  void SetModel(SequencerModel* model, ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions) const;

signals:
  void InstructionSelected(InstructionItem* selected);

private:
  void SetupConnections();

  NodeEditorToolBar* m_tool_bar{nullptr};
  GraphicsScene* m_graphics_scene{nullptr};
  GraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<GraphicsSceneController> m_scene_controller;

  SequencerModel* m_model{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H
