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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_SEQUENCER_OBSERVER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_SEQUENCER_OBSERVER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/core/dto_types_fwd.h>
#include <sup/sequencer/user_interface.h>

#include <gmock/gmock.h>

namespace testutils
{

//! Auxiliary class to control domain procedure execution.

class MockSequencerObserver : public userinterface_t
{
public:
  using vector_string_t = std::vector<std::string>;

  MOCK_METHOD(void, UpdateInstructionStatus, (const instruction_t* instruction), (override));
  MOCK_METHOD(void, VariableUpdated,
              (const std::string& name, const anyvalue_t& value, bool available), (override));

  MOCK_METHOD(bool, PutValue, (const anyvalue_t& value, const std::string& description),
              (override));
  MOCK_METHOD(bool, GetUserValue, (anyvalue_t & value, const std::string& description), (override));

  MOCK_METHOD(int, GetUserChoice, (const vector_string_t& options, const anyvalue_t& metadata),
              (override));

  MOCK_METHOD(void, Message, (const std::string& message), (override));
  MOCK_METHOD(void, Log, (int level, const std::string& str), (override));
};

using EmptyUserInterface = sup::sequencer::DefaultUserInterface;

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_SEQUENCER_OBSERVER_H_
