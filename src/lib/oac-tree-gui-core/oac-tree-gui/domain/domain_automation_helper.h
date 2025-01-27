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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_

//! @file
//! Collection of helper functions related to the oac-tree-server.

#include <oac-tree-gui/domain/sequencer_types_fwd.h>

#include <memory>

namespace sequencergui
{

/**
 * @brief Creates domain instruction from automation InstructionInfo presentation.
 *
 * This method ignores all possible children instructions.
 */
std::unique_ptr<sup::oac_tree::Instruction> CreateDomainInstruction(
    const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates domain variable from automation VariableInfo presentation.
 */
std::unique_ptr<sup::oac_tree::Variable> CreateDomainVariable(
    const sup::oac_tree::VariableInfo& info);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_AUTOMATION_HELPER_H_
