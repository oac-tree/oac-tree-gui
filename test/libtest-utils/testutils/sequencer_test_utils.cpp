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

#include "sequencer_test_utils.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_helper.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <sup/auto-server/instruction_map.h>
#include <sup/auto-server/job_info.h>
#include <sup/auto-server/job_utils.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

#include <testutils/test_utils.h>

namespace
{

const std::string db_content = R"RAW(
record (bo,"CA-TESTS:BOOL")
{
    field(DESC,"Some EPICSv3 record")
    field(ONAM,"TRUE")
    field(OSV,"NO_ALARM")
    field(ZNAM,"FALSE")
    field(ZSV,"NO_ALARM")
    field(VAL,"0")
}

record (longin,"CA-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"1")
}

record (ao,"CA-TESTS:FLOAT")
{
    field(DESC,"Some EPICSv3 record")
    field(DRVH,"5.0")
    field(DRVL,"-5.0")
    field(VAL,"0")
}

record (stringout,"CA-TESTS:STRING")
{
    field(DESC,"Some EPICSv3 record")
    field(VAL,"undefined")
}

record (waveform,"CA-TESTS:CHARRAY")
{
    field(DESC,"Some EPICSv3 record")
    field(FTVL, "CHAR")
    field(NELM, "1024")
}
)RAW";

std::unique_ptr<variable_t> CreateVariable(const std::string &type, const std::string &name,
                                           const sup::dto::AnyValue &initial_value,
                                           const std::string &channel_name = {})
{
  using namespace sequencergui::domainconstants;
  auto result = sequencergui::CreateDomainVariable(type);
  result->SetName(name);
  result->AddAttribute(kTypeAttribute, sup::gui::AnyTypeToJSONString(initial_value));
  result->AddAttribute(kValueAttribute, sup::gui::ValuesToJSONString(initial_value));
  if (!channel_name.empty())
  {
    result->AddAttribute(sequencergui::domainconstants::kChannelAttribute, channel_name);
  }
  return result;
}

}  // unnamed namespace

namespace testutils
{

std::string GetEpicsDBContentString()
{
  return db_content;
}

std::unique_ptr<variable_t> CreateLocalVariable(const std::string &name,
                                                const sup::dto::AnyValue &initial_value)
{
  return CreateVariable(sequencergui::domainconstants::kLocalVariableType, name, initial_value);
}

std::unique_ptr<variable_t> CreatePVAccessClientVariable(const std::string &name,
                                                         const sup::dto::AnyValue &initial_value,
                                                         const std::string &channel_name)
{
  return CreateVariable(sequencergui::domainconstants::kPvAccessClientVariableType, name,
                        initial_value, channel_name);
}

std::unique_ptr<variable_t> CreatePVAccessServerVariable(const std::string &name,
                                                         const sup::dto::AnyValue &initial_value,
                                                         const std::string &channel_name)
{
  return CreateVariable(sequencergui::domainconstants::kPvAccessServerVariableType, name,
                        initial_value, channel_name);
}

sup::auto_server::JobInfo CreateJobInfo(const std::string &procedure_text)
{
  const std::string prefix = "JobInfoTest:FromProcedure:";
  auto procedure =
      sup::sequencer::ParseProcedureString(testutils::CreateProcedureString(procedure_text));
  auto root = procedure->RootInstruction();
  const sup::auto_server::InstructionMap instr_map{root};
  auto job_info = sup::auto_server::utils::CreateJobInfo(prefix, *procedure, instr_map);

  return job_info;
}

}  // namespace testutils
