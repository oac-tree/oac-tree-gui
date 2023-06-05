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

#ifndef SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
#define SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_

#include <QWidget>
#include <memory>

class QSplitter;
class QShowEvent;

namespace mvvm
{
class ModelHasChangedController;
}

namespace sup::gui {
class CodeView;
}

namespace sequencergui
{
class SequencerModel;
class ComposerWidgetPanel;
class ComposerPanel;
class NodeEditor;
class ProcedureItem;
class ItemStackWidget;
class SequencerComposerActions;

//! Main widget for model editing. Contains a toolbar on top, and few widgets beneath:
//! a graphics scene representing a node editor, a toolbox with items to drag-and-drop on the
//! scene, property editor, and sample tree view. Belongs to MainWindow.

class SequencerComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerComposerView(QWidget* parent = nullptr);
  ~SequencerComposerView() override;

  void SetModel(SequencerModel* model);

protected:
  void showEvent(QShowEvent* event) override;

private:
  void SetupMenuActions();
  void UpdateXML();
  void SetupConnections();

  ProcedureItem* GetFirstProcedure();

  ComposerPanel* m_composer_panel{nullptr};
  NodeEditor* m_node_editor{nullptr};
  ItemStackWidget* m_central_panel{nullptr};
  ComposerWidgetPanel* m_composer_widget_panel{nullptr};
  sup::gui::CodeView* m_xml_view{nullptr};
  ItemStackWidget* m_right_panel{nullptr};

  std::unique_ptr<mvvm::ModelHasChangedController> m_model_changed_controller;

  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
  bool m_block_selection_to_scene{false};

  SequencerComposerActions* m_composer_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
