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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_

#include <QWidget>

namespace sup::gui
{
class ItemStackWidget;
}

namespace sequencergui
{

class InstructionEditorWidget;
class InstructionItem;
class ProcedureItem;
class SequencerModel;
class WorkspaceEditorWidget;
class NodeEditor;
class XmlPanel;

/**
 * @brief The ComposerWidgetPanel class is a main vertical panel with a stack of widget for
 * procedure editing.
 *
 * It is located both at the center and on the right of SequencerComposerView (a second main view).
 * Depending on the current selection can show
 * - instruction tree editor
 * - workspace editor
 * - node editor
 * - XML viewer
 */
class ComposerWidgetPanel : public QWidget
{
  Q_OBJECT

public:
  enum WidgetType
  {
    kInstructionTree,
    kWorkspace,
    kNodeEditor,
    kXmlPanel
  };

  explicit ComposerWidgetPanel(QWidget* parent = nullptr);
  ~ComposerWidgetPanel() override;

  void SetModel(SequencerModel* model);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

  WidgetType GetCurrentWidgetType() const;
  void SetCurrentWidgetType(WidgetType widget_type);

signals:
  void InstructionSelected(sequencergui::InstructionItem* selected);
  void ExportToFileRequest();

private:
  void SetupConnections();

  InstructionEditorWidget* m_instruction_editor_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_editor_widget{nullptr};
  NodeEditor* m_node_editor{nullptr};
  XmlPanel* m_xml_panel{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};

  ProcedureItem* m_procedure{nullptr};
  SequencerModel* m_model{nullptr};
  bool m_block_selection_notification{false};  //!< do not notify ouside if selection changed
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_
