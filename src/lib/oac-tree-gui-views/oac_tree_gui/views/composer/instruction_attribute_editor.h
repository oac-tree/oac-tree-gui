/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_H_

#include <QStringList>
#include <QWidget>
#include <functional>

class QTreeView;
class QToolBar;

namespace mvvm
{
class SessionItem;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
}

namespace oac_tree_gui
{

class AttributeEditorActions;
class AttributeEditorActionHandler;
class AttributeEditorContext;

/**
 * @brief The InstructionAttributeEditor provides a toolbar and three-column tree for editing
 * instruction attributes.
 *
 * It is located at the bottom of the InstructionEditorWidget (main SequencerComposerView).
 */
class InstructionAttributeEditor : public QWidget
{
  Q_OBJECT

public:
  using variable_names_func_t = std::function<QStringList()>;
  explicit InstructionAttributeEditor(variable_names_func_t variable_names_func,
                                      QWidget* parent_widget = nullptr);
  ~InstructionAttributeEditor() override;

  void SetInstruction(mvvm::SessionItem* instruction_item);

signals:
  void EditAnyvalueRequest();

private:
  void SummonContextMenu(const QPoint& point);
  AttributeEditorContext CreateAttributeEditorActionContext();
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateTreeComponentProvider();

  variable_names_func_t m_variable_names_func;

  QToolBar* m_tool_bar{nullptr};
  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  std::unique_ptr<AttributeEditorActionHandler> m_attribute_action_handler;
  AttributeEditorActions* m_attribute_actions{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_H_
