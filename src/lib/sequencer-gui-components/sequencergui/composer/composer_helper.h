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

/**
 * @brief The ObjectGroupInfo class represents a named group of object names.
 */
struct ObjectGroupInfo
{
  std::string group_name;
  std::vector<std::string> object_names;
};

/**
 * @brief Creates a collection of sequencer instruction types grouped according to a plugin name.
 */
std::vector<ObjectGroupInfo> CreateInstructionTypeGroups();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSER_HELPER_H_
