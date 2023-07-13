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

#ifndef SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPER_H_

//! Collection of helper functions to transform VariableItem and InstructionItem to/from domain.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>

namespace mvvm
{
class CompoundItem;
}

namespace sup::gui
{
class AnyValueItem;
class AnyValueScalarItem;
}  // namespace sup::gui

namespace sequencergui
{

class VariableItem;
class ProcedurePreambleItem;

//! Sets AnyValueItem on board of \it variable_item using given \it anyvalue.
//! If AnyValueItem already exist, it will be replaced.
void SetAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

//! Sets AnyValueItem on board of \it variable_item using \it json_type provided.
void SetAnyValueFromJsonType(const std::string& json_type, VariableItem& variable_item);

//! Sets AnyValueItem on board of variable item using domain variable.
void SetAnyValueFromDomainVariable(const variable_t& variable, VariableItem& variable_item,
                                   const anytype_registry_t* registry = nullptr);

//! Updates existing AnyValueItem on board of \it variable_item using given \it anyvalue.
//! It is expected that the layout of existing AnyValueItem matches the layout of \it anyvalue.
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

//! Adds an attribute to given value. If the value is empty string, do nothing.
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          variable_t& variable);

//! Sets JSON type attribute from given \it item to the domain variable.
void SetJsonTypeAttribute(const VariableItem& item, variable_t& variable);

//! Sets JSON value attribute from given \it item to the domain variable.
void SetJsonValueAttribute(const VariableItem& item, variable_t& variable);

//! Adds an attribute to given value. If the value is empty string, do nothing.
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          instruction_t& instruction);

/**
 * @brief Add property to item using sequencer attribute definition.
 *
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @return Returns item representing a new property.
 */
sup::gui::AnyValueItem* AddPropertyFromDefinition(const attribute_definition_t& attr,
                                                  mvvm::CompoundItem& item);

/**
 * @brief Set property item from the domain attribute.
 *
 * @tparam Domain instruction or variable.
 *
 * @param variable Domain object to set from.
 * @param attribute_name The attribute name of the domain object.
 * @param item An item representing an attribute.
 *
 */

template <typename T>
void SetPropertyFromDomainAttribute(const T& domain, const std::string& attribute_name,
                                    sup::gui::AnyValueScalarItem& item);

extern template void SetPropertyFromDomainAttribute<variable_t>(const variable_t& domain,
                                                                const std::string& attribute_name,
                                                                sup::gui::AnyValueScalarItem& item);
extern template void SetPropertyFromDomainAttribute<instruction_t>(
    const instruction_t& domain, const std::string& attribute_name,
    sup::gui::AnyValueScalarItem& item);

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
void SetDomainAttribute(const sup::gui::AnyValueScalarItem& item, const std::string& attribute_name,
                        T& domain);

extern template void SetDomainAttribute<variable_t>(const sup::gui::AnyValueScalarItem& item,
                                                    const std::string& attribute_name,
                                                    variable_t& domain);
extern template void SetDomainAttribute<instruction_t>(const sup::gui::AnyValueScalarItem& item,
                                                       const std::string& attribute_name,
                                                       instruction_t& domain);

/**
 * @brief Register tag in instruction item to be able to add other instructions as children.
 *
 * @param instruction The domain instruction is used to deduce category (compound, decorator).
 * @param item An item where tag will be registered.
 */
void RegisterChildrenTag(const instruction_t& instruction, mvvm::CompoundItem& item);

/**
 * @brief Populates domain preamble from ProcedurePreambleItem.
 */
void PopulateProcedurePreamble(const ProcedurePreambleItem& item, preamble_t& preamble);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPER_H_
