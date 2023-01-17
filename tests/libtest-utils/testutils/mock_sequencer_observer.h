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

#ifndef MOCKSEQUENCEROBSERVER_H
#define MOCKSEQUENCEROBSERVER_H

#include <gmock/gmock.h>
#include <sequencergui/domain/sequencer_types.h>

#include <sup/gui/dto/dto_types_fwd.h>
#include <sup/sequencer/user_interface.h>

namespace testutils
{

//! Auxiliary class to control domain procedure execution.

class MockSequencerObserver : public userinterface_t
{
public:
  MOCK_METHOD(void, UpdateInstructionStatusImpl, (const instruction_t* instruction), (override));
  MOCK_METHOD(void, VariableUpdatedImpl, (const std::string& name, const anyvalue_t& value),
              (override));

  MOCK_METHOD(bool, PutValueImpl, (const anyvalue_t& value, const std::string& description),
              (override));
  MOCK_METHOD(bool, GetUserValueImpl, (anyvalue_t & value, const std::string& description),
              (override));

  MOCK_METHOD(int, GetUserChoiceImpl,
              (const std::vector<std::string>& choices, const std::string& description),
              (override));
  MOCK_METHOD(void, StartSingleStepImpl, (), (override));
  MOCK_METHOD(void, EndSingleStepImpl, (), (override));

  MOCK_METHOD(void, MessageImpl, (const std::string& message), (override));
  MOCK_METHOD(void, LogImpl, (int level, const std::string& str), (override));
};

}  // namespace testutils

#endif  //  MOCKSEQUENCEROBSERVER_H
