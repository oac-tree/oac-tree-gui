/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_UTILS_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_UTILS_H_

//! @file anyvalue_buildnode_utils.h
//! Collection of utility functions to support AbstractAnyValueBuildNode and Co.

#include <memory>
#include <stack>

namespace anyvalueeditor
{

class AbstractAnyValueBuildNode;

//! Returns true if it is possible to add value node. This will be the case in one of three cases:
//! 1) the stack is empty 2) last node denotes that we are about to add a field to the struct
//! 3) last node denote that we are about to add an element to the array.
bool CanAddValueNode(const std::stack<std::unique_ptr<AbstractAnyValueBuildNode>>& stack);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_UTILS_H_
