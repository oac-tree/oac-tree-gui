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

#ifndef OAC_TREE_GUI_COMPOSER_I_WORKSPACE_EDITOR_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_I_WORKSPACE_EDITOR_ACTION_HANDLER_H_

#include <string>

namespace oac_tree_gui
{

/**
 * @brief The WorkspaceEditorActionHandler class is an interface to implement all operations with
 * sequencer workspace variables.
 */
class IWorkspaceEditorActionHandler
{
public:
  IWorkspaceEditorActionHandler() = default;
  virtual ~IWorkspaceEditorActionHandler() = default;

  IWorkspaceEditorActionHandler(const IWorkspaceEditorActionHandler&) = delete;
  IWorkspaceEditorActionHandler& operator=(const IWorkspaceEditorActionHandler&) = delete;
  IWorkspaceEditorActionHandler(IWorkspaceEditorActionHandler&&) = delete;
  IWorkspaceEditorActionHandler& operator=(IWorkspaceEditorActionHandler&&) = delete;

  /**
   * @brief Adds variable to the workspace.
   */
  virtual void AddVariableRequest(const std::string& variable_type_name) = 0;

  /**
   * @brief Checks if variable can be removed.
   */
  virtual bool CanRemoveVariable() const = 0;

  /**
   * @brief Removes currently selected variable.
   */
  virtual void RemoveVariable() = 0;

  /**
   * @brief Calls external AnyValue editor for currently selected attribute, if possible.
   */
  virtual void EditAnyValue() = 0;

  /**
   * @brief Checks if cut operation is possible.
   */
  virtual bool CanCut() const = 0;

  /**
   * @brief Cut selected variable.
   */
  virtual void Cut() = 0;

  /**
   * @brief Checks if copy operation is possible.
   */
  virtual bool CanCopy() const = 0;

  /**
   * @brief Copy selected variable.
   */
  virtual void Copy() = 0;

  /**
   * @brief Checks if paste operation is possible.
   */
  virtual bool CanPaste() const = 0;

  /**
   * @brief Paste new variable after currently selected variable.
   */
  virtual void Paste() = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_I_WORKSPACE_EDITOR_ACTION_HANDLER_H_
