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

#include "anyvalueeditor/anyvalue_buildnodes.h"

namespace anyvalueeditor
{

AnyValueBuildNode::AnyValueBuildNode(sup::dto::AnyValue &&value)
    : AbstractAnyValueBuildNode(std::move(value))
{
}

bool AnyValueBuildNode::Process(std::stack<node_t> &stack)
{
  return true;
}

}  // namespace anyvalueeditor
