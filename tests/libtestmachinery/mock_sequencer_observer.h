/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <sequencergui/domain/sequencer_types.h>
#include <sup/sequencer/user_interface.h>

namespace ccs::types
{
class AnyValue;
}
using anyvalue_t = ccs::types::AnyValue;

#include <gmock/gmock.h>

namespace testutils
{

//! Auxiliary class to control domain procedure execution.

class MockSequencerObserver : public userinterface_t
{
public:
  MOCK_METHOD(void, UpdateInstructionStatusImpl, (const instruction_t* instruction));
  MOCK_METHOD(void, VariableUpdatedImpl, (const std::string& name, const anyvalue_t& value));

  MOCK_METHOD(bool, PutValueImpl, (const anyvalue_t& value, const std::string& description));
  MOCK_METHOD(bool, GetUserValueImpl, (const anyvalue_t& value, const std::string& description));

  MOCK_METHOD(int, GetUserChoiceImpl,
              (const std::vector<std::string>& choices, const std::string& description));
  MOCK_METHOD(void, StartSingleStepImpl, ());
  MOCK_METHOD(void, EndSingleStepImpl, ());

  MOCK_METHOD(void, MessageImpl, (const std::string& message));
};

}  // namespace testutils

#endif  //  MOCKSEQUENCEROBSERVER_H
