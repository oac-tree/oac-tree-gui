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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_

#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/composer/instruction_editor_context.h>

#include <gmock/gmock.h>

#include <memory>

namespace oac_tree_gui
{
class InstructionEditorActionHandler;
}

namespace oac_tree_gui::test
{

/**
 * @brief The MockInstructionEditorContext creates and listens callbacks used by
 * InstructionEditorActionHandler.
 */
class MockInstructionEditorContext
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
   * @param instruction_container Current container with instructions.
   * @param current_selection The instructions which will be reported as selected by the user.
   */
  InstructionEditorContext CreateContext(InstructionContainerItem* instruction_container,
                                         const std::vector<InstructionItem*>& current_selection);

  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(
      InstructionContainerItem* instruction_container,
      const std::vector<InstructionItem*>& current_selection);

  QMimeData* GetClipboardContent() const;

  void SetClipboardContent(std::unique_ptr<QMimeData> clipboard);

  std::vector<mvvm::SessionItem*> GetNotifyRequests();

  void SetAsCurrentSelection(const std::vector<InstructionItem*>& selection);

  std::unique_ptr<QMimeData> m_clipboard_content;
  std::vector<InstructionItem*> m_current_selection;
  std::vector<mvvm::SessionItem*> m_notify_requests;
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_
