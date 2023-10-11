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

#ifndef SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem and InstructionItem properties
//! from/to domain attributes.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <string>

namespace mvvm
{
class SessionItem;
class CompoundItem;
}  // namespace mvvm

namespace sequencergui
{

/**
 * @brief Returns true if given attribute value is referring to `$placeholder`.
 */
bool IsPlaceholderAttribute(const std::string& attribute_value);

/**
 * @brief Returns true if given attribute value is referring to `@variable`.
 */
bool IsReferenceAttribute(const std::string& attribute_value);

/**
 * @brief Add property to item using sequencer attribute definition.
 *
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @return Returns item representing a new property.
 */
mvvm::SessionItem* AddPropertyFromDefinitionV2(const attribute_definition_t& attr,
                                             mvvm::CompoundItem& item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
