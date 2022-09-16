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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_H_

#include <anyvalueeditor/abstract_anyvalue_buildnode.h>
#include <anyvalueeditor/dto_types_fwd.h>

namespace anyvalueeditor
{

//! The node for AnyValueBuildAdapter to build AnyValue.

class AnyValueBuildNode : public AbstractAnyValueBuildNode
{
public:
  AnyValueBuildNode(sup::dto::AnyValue&& value);

  bool Process(std::stack<node_t>& stack);
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_H_
