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

#include "sequencer_test_utils.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/variable_item.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/model_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/instruction_map.h>
#include <sup/oac-tree/job_info.h>
#include <sup/oac-tree/job_info_utils.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

#include <testutils/test_utils.h>

namespace
{

const std::string db_content = R"RAW(
record (bo,"SEQUENCERGUI-CA-TESTS:BOOL")
{
    field(DESC,"Some EPICSv3 record")
    field(ONAM,"TRUE")
    field(OSV,"NO_ALARM")
    field(ZNAM,"FALSE")
    field(ZSV,"NO_ALARM")
    field(VAL,"0")
}

record (longin,"SEQUENCERGUI-CA-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"1")
}

record (longout,"SEQUENCERGUI-CA-WORKSPACE-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"42")
}

record (ao,"SEQUENCERGUI-CA-TESTS:FLOAT")
{
    field(DESC,"Some EPICSv3 record")
    field(DRVH,"5.0")
    field(DRVL,"-5.0")
    field(VAL,"0")
}

record (stringout,"SEQUENCERGUI-CA-TESTS:STRING")
{
    field(DESC,"Some EPICSv3 record")
    field(VAL,"undefined")
}

record (waveform,"SEQUENCERGUI-CA-TESTS:CHARRAY")
{
    field(DESC,"Some EPICSv3 record")
    field(FTVL, "CHAR")
    field(NELM, "1024")
}
)RAW";

std::unique_ptr<variable_t> CreateVariable(const std::string &variable_type,
                                           const std::string &name,
                                           const sup::dto::AnyValue &initial_value,
                                           const std::string &channel_name = {})
{
  using namespace oac_tree_gui::domainconstants;
  auto result = oac_tree_gui::CreateDomainVariable(variable_type);
  result->SetName(name);
  result->AddAttribute(kTypeAttribute, sup::gui::AnyTypeToJSONString(initial_value));
  result->AddAttribute(kValueAttribute, sup::gui::ValuesToJSONString(initial_value));
  if (!channel_name.empty())
  {
    result->AddAttribute(oac_tree_gui::domainconstants::kChannelAttribute, channel_name);
  }
  return result;
}

}  // unnamed namespace

namespace oac_tree_gui::test
{

std::string GetEpicsDBContentString()
{
  return db_content;
}

std::unique_ptr<variable_t> CreateLocalVariable(const std::string &name,
                                                const sup::dto::AnyValue &initial_value)
{
  return CreateVariable(oac_tree_gui::domainconstants::kLocalVariableType, name, initial_value);
}

std::unique_ptr<variable_t> CreatePVAccessClientVariable(const std::string &name,
                                                         const sup::dto::AnyValue &initial_value,
                                                         const std::string &channel_name)
{
  return CreateVariable(oac_tree_gui::domainconstants::kPvAccessClientVariableType, name,
                        initial_value, channel_name);
}

std::unique_ptr<variable_t> CreatePVAccessServerVariable(const std::string &name,
                                                         const sup::dto::AnyValue &initial_value,
                                                         const std::string &channel_name)
{
  return CreateVariable(oac_tree_gui::domainconstants::kPvAccessServerVariableType, name,
                        initial_value, channel_name);
}

std::unique_ptr<variable_t> CreateChannelAccessVariable(const std::string &name,
                                                        const sup::dto::AnyValue &initial_value,
                                                        const std::string &channel_name)
{
  return CreateVariable(oac_tree_gui::domainconstants::kChannelAccessVariableType, name,
                        initial_value, channel_name);
}

sup::oac_tree::JobInfo CreateJobInfo(const std::string &procedure_text)
{
  auto procedure = sup::oac_tree::ParseProcedureString(test::CreateProcedureString(procedure_text));
  procedure->Setup();
  auto root = procedure->RootInstruction();
  const sup::oac_tree::InstructionMap instr_map{root};
  auto job_info = sup::oac_tree::utils::CreateJobInfo(*procedure, instr_map);

  return job_info;
}

bool IsEqual(const oac_tree_gui::VariableItem &variable, const sup::dto::AnyValue &value)
{
  if (auto anyvalue_item = variable.GetAnyValueItem(); anyvalue_item)
  {
    return sup::gui::CreateAnyValue(*anyvalue_item) == value;
  }
  return false;
}

std::vector<InstructionItem *> FindInstructions(const mvvm::ISessionModel &model,
                                                const std::string &domain_type)
{
  std::vector<InstructionItem *> result;
  auto candidates = mvvm::utils::FindItems<InstructionItem>(&model);
  for (auto instruction : candidates)
  {
    if (instruction->GetDomainType() == domain_type)
    {
      result.push_back(instruction);
    }
  }
  return result;
}

}  // namespace oac_tree_gui::test
