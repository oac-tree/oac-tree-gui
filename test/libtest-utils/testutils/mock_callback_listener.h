/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_CALLBACK_LISTENER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_CALLBACK_LISTENER_H_

#include <gmock/gmock.h>

#include <functional>

namespace testutils
{

//! Convenience class to test callbacks with gmock. Template parameter defines callback signature
//! std::function(void(const T&)).

//! @code{.cpp}
//! MockCallbackListener<WorkspaceEvent> listener;
//!
//! auto callback = listener.CreateCallback();
//!
//! EXPECT_CALL(listener, OnCallback(_)).Times(1);
//!
//! callback(workspace_event); // using somewhere to check expectations
//! @endcode

template <typename T>
class MockCallbackListener
{
public:
  std::function<void(const T&)> CreateCallback()
  {
    return [this](const T& arg) { OnCallback(arg); };
  }

  MOCK_METHOD(void, OnCallback, (const T& arg));
};

}

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_CALLBACK_LISTENER_H_
