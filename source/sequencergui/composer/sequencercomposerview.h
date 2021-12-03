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
  bool m_block_selection_to_scene{false};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCERCOMPOSERVIEW_H
