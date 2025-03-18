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

#include <oac_tree_gui/composer/i_workspace_editor_action_handler.h>
#include <oac_tree_gui/composer/workspace_editor_context.h>

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
class WorkspaceEditorActionHandler : public IWorkspaceEditorActionHandler
{
public:
  explicit WorkspaceEditorActionHandler(WorkspaceEditorContext context);

  void AddVariable(const std::string& variable_type_name) override;

  bool CanRemoveVariable() const override;

  void RemoveVariable() override;

  void EditAnyValue() override;

  bool CanCut() const override;

  void Cut() override;

  bool CanCopy() const override;

  void Copy() override;

  bool CanPaste() const override;

  void Paste() override;

  VariableItem* GetSelectedVariable() const;

  std::vector<VariableItem*> GetSelectedVariables() const;

private:
  mvvm::ISessionModel* GetModel() const;
  WorkspaceItem* GetWorkspaceItem() const;
  void SelectNotify(mvvm::SessionItem* item) const;

  const QMimeData* GetMimeData() const;

  void UpdateProcedurePreamble();

  void InsertVariableAfterCurrentSelection(
      std::vector<std::unique_ptr<mvvm::SessionItem> > variable_items);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  WorkspaceItem* m_workspace{nullptr};
  WorkspaceEditorContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_ACTION_HANDLER_H_
