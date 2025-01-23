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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_USER_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_USER_CONTEXT_H_

#include <oac-tree-gui/jobsystem/user_context.h>

#include <gmock/gmock.h>

namespace sequencergui::test
{

/**
 * @brief The MockUserContext class checks callbacks related to user input requests.
 */
class MockUserContext
{
public:
  MOCK_METHOD(sequencergui::UserInputResult, OnUserInputRequest,
              (const sequencergui::UserInputArgs&), (const));
  MOCK_METHOD(sequencergui::UserChoiceResult, OnUserChoiceRequest,
              (const sequencergui::UserChoiceArgs&), (const));

  /**
   * @brief Creates a structure with callbacks to trigger mock methods.
   */
  sequencergui::UserContext CreateContext()
  {
    sequencergui::UserContext result;

    result.user_input_callback = [this](const sequencergui::UserInputArgs& args)
    { return OnUserInputRequest(args); };

    result.user_choice_callback = [this](const sequencergui::UserChoiceArgs& args)
    { return OnUserChoiceRequest(args); };

    return result;
  }
};

}  // namespace sequencergui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
