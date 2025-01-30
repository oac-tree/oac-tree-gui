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

#include <oac_tree_gui/jobsystem/user_context.h>

#include <gmock/gmock.h>

namespace oac_tree_gui::test
{

/**
 * @brief The MockUserContext class checks callbacks related to user input requests.
 */
class MockUserContext
{
public:
  MOCK_METHOD(oac_tree_gui::UserInputResult, OnUserInputRequest,
              (const oac_tree_gui::UserInputArgs&), (const));
  MOCK_METHOD(oac_tree_gui::UserChoiceResult, OnUserChoiceRequest,
              (const oac_tree_gui::UserChoiceArgs&), (const));

  /**
   * @brief Creates a structure with callbacks to trigger mock methods.
   */
  oac_tree_gui::UserContext CreateContext()
  {
    oac_tree_gui::UserContext result;

    result.user_input_callback = [this](const oac_tree_gui::UserInputArgs& args)
    { return OnUserInputRequest(args); };

    result.user_choice_callback = [this](const oac_tree_gui::UserChoiceArgs& args)
    { return OnUserChoiceRequest(args); };

    return result;
  }
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
