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

#ifndef SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem and InstructionItem properties
//! from automation server information.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>
#include <vector>

namespace sequencergui
{

class InstructionItem;

/**
 * @brief The InstructionTree class contains a root instruction with all its children, and the
 * correspondence of automation index to instruction pointer.
 *
 * It is obtained from sup::auto_server::InstructionInfo.
 */
struct InstructionTree
{
  std::unique_ptr<InstructionItem> instruction; //!< root instruction
  std::vector<const InstructionItem*> indexes; //!<
};

/**
 * @brief Creates InstructionItem from the automation server information.
 *
 * This method ignores all possible children instructions.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(
    const sup::auto_server::InstructionInfo& info);

/**
 * @brief Creates InstructionItem with all its children from the automation server information.
 */
InstructionTree CreateInstructionItemTree(
    const sup::auto_server::InstructionInfo& info);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_
