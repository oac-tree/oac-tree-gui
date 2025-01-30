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

#ifndef SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_
#define SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_

//! @file
//! Collection of utility functions to convert sup::oac_tree::Procedure into SessionModel.

#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <oac-tree-gui/model/runner_status.h>

#include <sup/oac-tree/job_states.h>

#include <memory>
#include <string>

namespace oac_tree_gui
{

class InstructionItem;
class VariableItem;

/**
 * @brief Creates VariableItem from string representing the type of sup::oac_tree::Variable.
 */
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

/**
 * @brief Creates InstructionItem from string representing Type of sup::oac_tree::Instruction.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

/**
 * @brief Returns GUI runner status from domain job state.
 */
RunnerStatus GetRunnerStatusFromDomain(sup::oac_tree::JobState job_state);

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_
