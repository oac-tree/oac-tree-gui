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

#include "UserInterface.h"
#include <sequencergui/model/sequencer_types.h>

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
  MOCK_METHOD1(UpdateInstructionStatusImpl, void(const instruction_t* instruction));
  MOCK_METHOD2(VariableUpdatedImpl, void(const std::string& name, const anyvalue_t& value));

  MOCK_METHOD2(PutValueImpl, bool(const anyvalue_t& value, const std::string& description));
  MOCK_METHOD2(GetUserValueImpl, bool(const anyvalue_t& value, const std::string& description));

  MOCK_METHOD2(GetUserChoiceImpl,
               int(const std::vector<std::string>& choices, const std::string& description));
  MOCK_METHOD0(StartSingleStepImpl, void(void));
  MOCK_METHOD0(EndSingleStepImpl, void(void));

  MOCK_METHOD1(MessageImpl, void(const std::string& message));
};

}  // namespace testutils

#endif  //  MOCKSEQUENCEROBSERVER_H
