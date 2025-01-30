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

#ifndef OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_ACTION_HANDLER_H_

#include <oac-tree-gui/composer/workspace_editor_context.h>

#include <QObject>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

class VariableItem;
class WorkspaceItem;

/**
 * @brief The WorkspaceEditorActionHandler class logic to add/remove/edit variables, and to prform
 * standard cut/copy/paster operations.
 */
class WorkspaceEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit WorkspaceEditorActionHandler(WorkspaceEditorContext context, QObject* parent_object = nullptr);

  void OnAddVariableRequest(const std::string& variable_type_name);

  /**
   * @brief Checks if variable can be removed.
   */
  bool CanRemoveVariable() const;

  /**
   * @brief Removes currently selected variable.
   */
  void OnRemoveVariableRequest();

  /**
   * @brief Calls external AnyValue editor for currently selected attribute, if possible.
   */
  void OnEditAnyValueRequest();

  /**
   * @brief Checks if cut operation is possible.
   */
  bool CanCut() const;

  /**
   * @brief Cut selected variable.
   */
  void Cut();

  /**
   * @brief Checks if copy operation is possible.
   */
  bool CanCopy() const;

  /**
   * @brief Copy selected variable.
   */
  void Copy();

  /**
   * @brief Checks if paste operation is possible.
   */
  bool CanPaste() const;

  /**
   * @brief Paste new variable after currently selected variable.
   */
  void Paste();

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  mvvm::ISessionModel* GetModel() const;
  WorkspaceItem* GetWorkspaceItem() const;
  VariableItem* GetSelectedVariable() const;

  const QMimeData* GetMimeData() const;

  /**
   * @brief Inserts variable after currently selected item.
   */
  void InsertVariableAfterCurrentSelection(std::unique_ptr<mvvm::SessionItem> variable_item);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  WorkspaceItem* m_workspace{nullptr};
  WorkspaceEditorContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_ACTION_HANDLER_H_
