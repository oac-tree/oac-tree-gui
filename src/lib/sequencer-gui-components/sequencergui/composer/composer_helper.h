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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSER_HELPER_H_
#define SEQUENCERGUI_COMPOSER_COMPOSER_HELPER_H_

//! @file
//! Collection of helper functions for instruction and variable composition.

#include <string>
#include <vector>

namespace sequencergui
{

class DomainObjectTypeRegistry;

//! Collection of group names for available instructions.
const std::string kCoreGroup = "Core";
const std::string kMathGroup = "Math";
const std::string kEPICSGroup = "EPICS";
const std::string kControlGroup = "Control";
const std::string kConfigGroup = "SUP Configuration";
const std::string kMiscGroup = "Miscellaneous";

/**
 * @brief The ObjectGroupInfo class represents a named group of object names.
 */
struct ObjectGroupInfo
{
  std::string group_name;
  std::vector<std::string> object_names;
};

/**
 * @brief Creates a collection of sequencer instruction types grouped according to a plugin name
 * using given registry.
 *
 * A single group can contain instructions from several plugins.
 *
 * @param type_registry The registry with all loaded types.
 */
std::vector<ObjectGroupInfo> CreateInstructionTypeGroups(
    const DomainObjectTypeRegistry& type_registry);

/**
 * @brief Creates a collection of global registered sequencer instruction types grouped according to
 * a plugin name.
 *
 * This method uses global object type registry. A single group can contain instructions from
 * several plugins.
 *
 * @param type_registry The registry with all loaded types.
 */
std::vector<ObjectGroupInfo> CreateInstructionTypeGroups();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSER_HELPER_H_
