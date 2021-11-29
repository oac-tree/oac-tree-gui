/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/domainutils.h"

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "Variable.h"
#include "VariableRegistry.h"

namespace sequi::DomainUtils
{
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name)
{
  auto result = ::sup::sequencer::GlobalInstructionRegistry().Create(domain_name);
  if (!result)
  {
    throw std::runtime_error("Error in GlobalInstructionRegistry: can create instruction '"
                             + domain_name + "'");
  }
  return result;
}

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name)
{
  auto result = ::sup::sequencer::GlobalVariableRegistry().Create(domain_name);
  if (!result)
  {
    throw std::runtime_error("Error in GlobalVariableRegistry: can create variable '" + domain_name
                             + "'");
  }
  return result;
}

std::vector<std::string> GetDomainInstructionNames()
{
  return ::sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
}

}  // namespace sequi::DomainUtils
