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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_

#include <gmock/gmock.h>
#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/composer/instruction_editor_context.h>

namespace oac_tree_gui::test
{

/**
 * @brief The MockInstructionEditorContext creates and listens callbacks used by
 * InstructionEditorActionHandler.
 */
class MockInstructionEditorContext
{
public:
  MOCK_METHOD(ProcedureItem*, OnSelectedProcedure, (), ());
  MOCK_METHOD(InstructionItem*, OnSelectedInstruction, (), ());
  MOCK_METHOD(void, OnMessage, (const sup::gui::MessageEvent&), ());
  MOCK_METHOD(AnyValueDialogResult, OnEditAnyvalue, (const sup::gui::AnyValueItem*), ());
  MOCK_METHOD(const QMimeData*, OnGetMimeData, (), ());
  MOCK_METHOD(void, OnSetMimeData, (std::unique_ptr<QMimeData>), ());

  oac_tree_gui::InstructionEditorContext CreateContext();
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_INSTRUCTION_EDITOR_CONTEXT_H_
