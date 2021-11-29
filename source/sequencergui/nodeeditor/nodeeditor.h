/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H
#define SEQUENCERGUI_NODEEDITOR_NODEEDITOR_H

#include <QWidget>

class QKeyEvent;
class QToolBar;

namespace sequi
{
class GraphicsView;
class GraphicsScene;
class SequencerModel;
class GraphicsSceneController;
class InstructionItem;
class NodeEditorToolBar;
class ProcedureItem;

//! NodeEditor widget (central part of SequencerComposerView).

class NodeEditor : public QWidget
{
  Q_OBJECT

public:
  NodeEditor(QWidget* parent = nullptr);
  ~NodeEditor();

  void SetModel(SequencerModel* model, ProcedureItem* procedure);

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
