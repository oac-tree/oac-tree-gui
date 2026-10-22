/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_
#define OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_

//! Collection of constants for sequencer items.

#include <mvvm/core/mvvm_types.h>

#include <cstdint>

namespace oac_tree_gui::itemconstants
{

//! Constants to register property items.
constexpr auto kStatus = "kStatus";
constexpr auto kDescription = "description";
constexpr auto kXpos = "kXpos";
constexpr auto kYpos = "kYpos";
constexpr auto kChildInstructions = "kChildInstructions";
constexpr auto kAnyValueTag = "kAnyValueTag";
constexpr auto kAttributes = "kAttributes";
constexpr auto kIsAvailable = "connected";
constexpr auto kBreakpoint = "breakpoint";
constexpr auto kJobType = "kJobType";

constexpr std::int32_t kDomainTypeNameRole = 10;  // role to store domain type name

//! Role of AttributeItem: the attribute accepts text (`$par`, `@var`) instead of a value.
constexpr mvvm::role_t kAllowTextRole = mvvm::DataRole::kUser;

//! Role of AttributeItem: the text (`$par`, `@var`) used instead of the value.
constexpr mvvm::role_t kAttributeTextRole = mvvm::DataRole::kUser + 1;

constexpr auto kAnyValueDefaultDisplayName = "value";

constexpr auto kDefaultPlaceholderAttributeValue = "$par";

constexpr auto kTickTimeout = "kTickTimeout";

constexpr std::int32_t kDefaultTickTimeoutMsec = 20;

constexpr auto kBehaviorTag = "Behavior";
constexpr auto kNativeBehavior = "Native";
constexpr auto kHiddenBehavior = "Hidden";
constexpr auto kSucceedBehavior = "Succeed";
constexpr auto kFailBehavior = "Fail";

}  // namespace oac_tree_gui::itemconstants

#endif  // OAC_TREE_GUI_MODEL_ITEM_CONSTANTS_H_
