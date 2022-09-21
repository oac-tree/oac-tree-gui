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

#include "anyvalueeditor/anyvalue_buildnode_utils.h"

#include <anyvalueeditor/abstract_anyvalue_buildnode.h>
#include <mvvm/utils/container_utils.h>

#include <vector>

namespace anyvalueeditor
{

bool CanAddValueNode(const std::stack<std::unique_ptr<AbstractAnyValueBuildNode> > &stack)
{
  using NodeType = AbstractAnyValueBuildNode::NodeType;
  static const std::vector<NodeType> expected_types{NodeType::kStartArrayElement,
                                                    NodeType::kStartField};

  return stack.empty() || mvvm::utils::Contains(expected_types, stack.top()->GetNodeType());
}

}  // namespace anyvalueeditor
