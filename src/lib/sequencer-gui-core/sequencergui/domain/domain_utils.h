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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_

//! @file
//! Collection of utility functions to access/create various Sequencer-related objects. With this
//! file, we want to decrease the visibility of sequencer headers in the code as much as possible.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace sequencergui
{

/**
 * @brief Creates domain instruction from a given domain name.
 */
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name);

/**
 * @brief Creates domain variable from a given domain name.
 */
std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name);

/**
 * @brief Returns a vector of all existing instructions names in all currently loaded sequencer
 * plugins.
 */
std::vector<std::string> GetDomainInstructionNames();

/**
 * @brief Returns a vector of all existing variable names in all currently loaded sequencer
 * plugins.
 */
std::vector<std::string> GetDomainVariableNames();

/**
 * @brief Checks if the given sequencer variable type is available in currently loaded plugins.
 */
bool IsVariableTypeAvailable(const std::string& domain_type);

/**
 * @brief Checks if the given sequencer instruction type is available in currently loaded plugins.
 */
bool IsInstructionTypeAvailable(const std::string& domain_type);

/**
 * @brief Checks if EPICS plugin was loaded.
 */
bool IsSequencerPluginEpicsAvailable();

/**
 * @brief Checks if sup-conrtol plugin was loaded.
 */
bool IsSequencerControlPluginAvailable();

/**
 * @brief Extracts main text from the metadata presented by the given AnyValue.
 */
std::string GetMainTextFromMetadata(const anyvalue_t& metadata);

/**
 * @brief  Extracts title text from the metadata presented by the given AnyValue.
 */
std::string GetTitleTextFromMetadata(const anyvalue_t& metadata);

/**
 * @brief Checks if given AnyValue contains metadata suitable for user choice dialogs.
 */
bool IsSelectTextDialog(const anyvalue_t& metadata);

/**
 * @brief Checks if given AnyValue contains metadata suitable for message box dialogs.
 */
bool IsMessageBoxDialog(const anyvalue_t& metadata);

/**
 * @brief Loads all sequencer plugins and reports result of the load to the user.
 * @return Success flag and the error message.
 */
std::pair<bool, std::string> LoadPlugins();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_UTILS_H_
