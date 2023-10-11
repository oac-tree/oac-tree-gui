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
class CompoundItem;
}  // namespace mvvm

namespace sequencergui
{

class AttributeItem;

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
AttributeItem* AddPropertyFromDefinitionV2(const attribute_definition_t& attr,
                                           mvvm::CompoundItem& item);

/**
 * @brief Set property item from the domain attribute.
 *
 * @tparam Domain instruction or variable.
 *
 * @param variable Domain object to set from.
 * @param attribute_name The attribute name of the domain object.
 * @param item An item representing an attribute.
 */

template <typename T>
void SetPropertyFromDomainAttributeV2(const T& domain, const std::string& attribute_name,
                                      AttributeItem& item);

extern template void SetPropertyFromDomainAttributeV2<variable_t>(const variable_t& domain,
                                                                  const std::string& attribute_name,
                                                                  AttributeItem& item);
extern template void SetPropertyFromDomainAttributeV2<instruction_t>(
    const instruction_t& domain, const std::string& attribute_name, AttributeItem& item);

/**
 * @brief Set an attribute for domain variable from AnyValueScalarItem property.
 *
 * @tparam Domain instruction or variable.
 *
 * @param item The AnyValueScalarItem representing domain attribute.
 * @param attribute_name The name of the domain attribute to set.
 * @param variable The domain sequencer variable.
 */

template <typename T>
void SetDomainAttributeV2(const AttributeItem& item, const std::string& attribute_name, T& domain);

extern template void SetDomainAttributeV2<variable_t>(const AttributeItem& item,
                                                      const std::string& attribute_name,
                                                      variable_t& domain);
extern template void SetDomainAttributeV2<instruction_t>(const AttributeItem& item,
                                                         const std::string& attribute_name,
                                                         instruction_t& domain);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
