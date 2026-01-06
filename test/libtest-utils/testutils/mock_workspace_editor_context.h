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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_WORKSPACE_EDITOR_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_WORKSPACE_EDITOR_CONTEXT_H_

#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/composer/workspace_editor_context.h>

#include <gmock/gmock.h>

#include <memory>

namespace oac_tree_gui
{
class WorkspaceEditorActionHandler;
}

namespace oac_tree_gui::test
{

/**
 * @brief The MockWorkspaceEditorContext creates and listens callbacks used by
 * WorkspaceEditorActionHandler.
 */
class MockWorkspaceEditorContext
{
public:
  MOCK_METHOD(void, NotifyRequest, (mvvm::SessionItem*), ());
  MOCK_METHOD(void, OnMessage, (const sup::gui::MessageEvent&), ());
  MOCK_METHOD(AnyValueDialogResult, OnEditAnyvalue, (const sup::gui::AnyValueItem*), ());
  MOCK_METHOD(const QMimeData*, OnGetMimeData, (), ());
  MOCK_METHOD(void, OnSetMimeData, (), ());

  /**
   * @brief Creates context for InstructionEditorActionHandler.
   *
   * @param workspace The workspace which will be reported as selected by the user.
   * @param selected_items The list of items that will be reported as selected by the user.
   */
  WorkspaceEditorContext CreateContext(WorkspaceItem* workspace,
                                       const std::vector<mvvm::SessionItem*>& selection);

  /**
   * @brief Creates action handler.
   *
   * It is initialized with mock context pretending that the given procedure and instruction are
   * selected by the user.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(
      WorkspaceItem* workspace, const std::vector<mvvm::SessionItem*>& selection);

  QMimeData* GetClipboardContent() const;

  void SetClipboardContent(std::unique_ptr<QMimeData> clipboard);

  //!< here we save copy result reported via set_mime_data callback
  std::unique_ptr<QMimeData> m_clipboard_content;
  std::vector<mvvm::SessionItem*> m_current_selection;
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_WORKSPACE_EDITOR_CONTEXT_H_
