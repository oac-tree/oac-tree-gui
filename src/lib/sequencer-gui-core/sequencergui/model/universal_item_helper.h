/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_UNIVERSAL_ITEM_HELPER_H_
#define SEQUENCERGUI_MODEL_UNIVERSAL_ITEM_HELPER_H_

//! Helper functions to simplify construction of UniversalInstructionItem and UniversalVariableItem.

#include <memory>

namespace mvvm
{
class SessionModelInterface;
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

class InstructionItem;
class UniversalInstructionItem;

/**
 * @brief Convenience function to insert universal instruction in another instruction.
 * @param domain_type The domain instruction type.
 * @param model The model to insert
 * @return Inserted item
 */
UniversalInstructionItem* InsertInstruction(const std::string& domain_type,
                                            mvvm::SessionItem* parent);

/**
 * @brief Set input property to instruction item.
 */
void SetInput(const std::string& value, InstructionItem* item);

/**
 * @brief Set output property to instruction item.
 */
void SetOutput(const std::string& value, InstructionItem* item);

/**
 * @brief Set description property to instruction item.
 */
void SetDescription(const std::string& value, InstructionItem* item);

/**
 * @brief Set text attribute property to instruction item.
 */
void SetText(const std::string& value, InstructionItem* item);

/**
 * @brief Set variable name property to instruction item.
 */
void SetVariableName(const std::string& value, InstructionItem* item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_SEQUENCER_UTILS_H_
