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

#ifndef OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_
#define OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_

//! Collection of constants for sequencer items.

#include <string>

namespace oac_tree_gui::itemconstants
{

//! Constants to register property items.
const std::string kName = "name";
const std::string kStatus = "kStatus";
const std::string kDescription = "description";
const std::string kXpos = "kXpos";
const std::string kYpos = "kYpos";
const std::string kChildInstructions = "kChildInstructions";
const std::string kAnyValueTag = "kAnyValueTag";
const std::string kIsAvailable = "connected";
const std::string kBreakpoint = "breakpoint";
const std::string kJobType = "kJobType";

const int kDomainTypeNameRole = 10;  // role to store domain type name

const std::string kAnyValueDefaultDisplayName = "value";

const std::string kDefaultPlaceholderAttributeValue = "$par";

const std::string kTickTimeout = "kTickTimeout";

const int kDefaultTickTimeoutMsec = 20;

const std::string kBehaviorTag = "Behavior";
const std::string kNativeBehavior = "Native";
const std::string kHiddenBehavior = "Hidden";
const std::string kSucceedBehavior = "Succeed";
const std::string kFailBehavior = "Fail";

}  // namespace oac_tree_gui::itemconstants

#endif  // OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_
