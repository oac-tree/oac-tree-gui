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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_

//! @file
//! Collection of helper functions related to the sup-auto-server.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>

namespace sequencergui
{

/**
 * @brief Creates domain instruction from automation InstructionInfo presentation.
 *
 * This method ignores all possible children instructions.
 */
std::unique_ptr<sup::sequencer::Instruction> CreateDomainInstruction(
    const sup::sequencer::InstructionInfo& info);

/**
 * @brief Creates domain variable from automation VariableInfo presentation.
 */
std::unique_ptr<sup::sequencer::Variable> CreateDomainVariable(
    const sup::sequencer::VariableInfo& info);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_
