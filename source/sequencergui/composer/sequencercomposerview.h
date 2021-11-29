/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_SEQUENCERCOMPOSERVIEW_H
#define SEQUENCERGUI_COMPOSER_SEQUENCERCOMPOSERVIEW_H

#include <QWidget>
#include <memory>

class QSplitter;

namespace sequi
{
class SequencerModel;
class ComposerTreeWidget;
class ComposerPanel;
class NodeEditor;
class ProcedureItem;

//! Main widget for model editing. Contains a toolbar on top, and few widgets beneath:
//! a graphics scene representing a node editor, a toolbox with items to drag-and-drop on the
//! scene, property editor, and sample tree view. Belongs to MainWindow.

class SequencerComposerView : public QWidget
{
  Q_OBJECT

public:
  SequencerComposerView(QWidget* parent = nullptr);
  ~SequencerComposerView();

  void SetModel(SequencerModel* model);

private:
  void SetupConnections();
  ProcedureItem* GetFirstProcedure();

  ComposerPanel* m_composer_panel{nullptr};
  NodeEditor* m_node_editor{nullptr};
  ComposerTreeWidget* m_composer_tree_widget{nullptr};

  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCERCOMPOSERVIEW_H
