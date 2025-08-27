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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_HELPER_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_HELPER_H_

//! @file
//! Collection of utility functions to access/create various oac-tree-related objects. With this
//! file, we want to decrease the visibility of sequencer headers in the code as much as possible.

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace oac_tree_gui
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
 * @brief Returns basic plugin list.
 *
 * This is the list of plugins that are usually available both on CODAC and non-CODAC systems.
 */
std::vector<std::string> GetBasicPluginList();

/**
 * @brief Returns plugin list available only on CODAC systems.
 */
std::vector<std::string> GetSupPluginList();

/**
 * @brief Returns default plugin list.
 *
 * This is the list of plugins that are usually available both on CODAC and non-CODAC systems.
 */
std::vector<std::string> GetBasicPluginFileNames();

/**
 * @brief Converts the name of the plugin to library file name.
 *
 * Takes OS platform in the account.
 *
 * Linux: sequencer -> libsequencer.so
 *        libsequencer.so -> libsequencer.so
 *        libsequencer.dylib -> libsequencer.so
 *
 * MacOs: sequencer -> libsequencer.dylib
 *        libsequencer.so -> libsequencer.dylib
 *        libsequencer.dylib -> libsequencer.dylib
 *
 * Windows: not implemented, will return the same name
 */
std::string GetPluginFileName(const std::string& plugin_name);

/**
 * @brief Returns plugin name extracted from the given file name.
 *
 * It will remove path and "lib" prefix and ".so" or ".dylib" suffix on Linux/MacOs.
 */
std::string GetPluginNameFromFileName(const std::string& file_name);

/**
 * @brief Checks if given index represents a valid instruction index.
 */
bool IsValidInstructionIndex(sup::dto::uint32 index);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_HELPER_H_
