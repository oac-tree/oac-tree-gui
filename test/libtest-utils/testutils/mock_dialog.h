/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_DIALOG_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_DIALOG_H_

#include <sequencergui/components/anyvalue_editor_dialog_factory.h>

#include <gmock/gmock.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace testutils
{

/**
 * @brief The MockDialog class pretends to take an item for editing and returns the result to the
 * user.
 */
class MockDialog
{
public:
  void SetItemToReturn(sequencergui::AnyValueDialogResult dialog_result)
  {
    m_dialog_result = std::move(dialog_result);
  }

  MOCK_METHOD(void, OnEditingRequest, (const sup::gui::AnyValueItem* item));

  //! Creates a callback that mimicks editing request and returning the result to the user
  std::function<sequencergui::AnyValueDialogResult(const sup::gui::AnyValueItem*)> CreateCallback()
  {
    return [this](const sup::gui::AnyValueItem* item) -> sequencergui::AnyValueDialogResult
    {
      OnEditingRequest(item);
      return std::move(m_dialog_result);
    };
  }

  sequencergui::AnyValueDialogResult m_dialog_result;
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DIALOG_H_
