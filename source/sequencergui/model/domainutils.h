/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_DOMAINUTILS_H
#define SEQUENCERGUI_MODEL_DOMAINUTILS_H

//! @file domainutils.h
//! Collection of utility functions to access/create various Sequencer related objects.
//! With this file we want to decrease as much as possible the visibility of sequencer headers
//! in the code.

#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/sequencer_types.h"

#include "Variable.h"
#include "Instruction.h"

#include <memory>
#include <string>
#include <vector>

namespace sequi::DomainUtils
{

std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name);

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name);

std::vector<std::string> GetDomainInstructionNames();

}  // namespace sequi::DomainUtils

#endif  // SEQUENCERGUI_MODEL_DOMAINUTILS_H
