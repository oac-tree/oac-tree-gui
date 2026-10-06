/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_

#include <sup/gui/app/app_view_context.h>

#include <mvvm/model/item_selection.h>

#include <QWidget>
#include <memory>

class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class SettingsHeaderView;
class VisibilityAgentBase;
class SettingsSplitter;
}  // namespace sup::gui

namespace oac_tree_gui
{

class InstructionContainerItem;
class InstructionItem;
struct InstructionEditorContext;
class InstructionEditorActionHandler;
class InstructionEditorActions;
class InstructionAttributeEditor;
class CustomTreeViewStyle;

/**
 * @brief The InstructionEditorWidget is a main panel to compose instruction tree.
 *
 * Belongs to SequencerComposerView (the second main widget). Contains instruction tree on top, and
 * attribute editor at the bottom.
 */
class InstructionEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionEditorWidget(const sup::gui::AppViewContext& context,
                                   QWidget* parent_widget = nullptr);
  ~InstructionEditorWidget() override;

  InstructionEditorWidget(const InstructionEditorWidget&) = delete;
  InstructionEditorWidget& operator=(const InstructionEditorWidget&) = delete;
  InstructionEditorWidget(InstructionEditorWidget&&) = delete;
  InstructionEditorWidget& operator=(InstructionEditorWidget&&) = delete;

  void SetInstructionContainer(InstructionContainerItem* instruction_container);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  InstructionItem* GetSelectedInstruction() const;

  /**
   * @brief Insert instruction from toolbox to the instruction container.
   *
   * Where to insert will depend on the last selection.
   */
  void InsertInstructionFromToolBox(const QString& name);

signals:
  void InstructionSelected(const mvvm::ItemSelection& selection);

protected:
  void resizeEvent(QResizeEvent* event) override;

private:
  void SetupTree();
  void AdjustTreeAppearance();
  void SetInstructionContainerIntern(InstructionContainerItem* instruction_container);
  std::function<QStringList()> CreateVariableNameFunc();

  void SetupConnections();
  InstructionEditorContext CreateInstructionEditorContext();
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateProvider();

  void OnContextMenuRequest(const QPoint& point);

  sup::gui::AppViewContext m_context;

  QTreeView* m_tree_view{nullptr};
  sup::gui::SettingsHeaderView* m_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  InstructionAttributeEditor* m_attribute_editor{nullptr};

  sup::gui::SettingsSplitter* m_splitter{nullptr};
  InstructionContainerItem* m_instruction_container{nullptr};

  std::unique_ptr<InstructionEditorActionHandler> m_action_handler;
  InstructionEditorActions* m_editor_actions{nullptr};

  std::unique_ptr<sup::gui::VisibilityAgentBase> m_visibility_agent;
  CustomTreeViewStyle* m_tree_view_style{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_
