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

#include <anyvalueeditor/abstract_anyvalue_buildnode.h>

#include <stdexcept>

namespace anyvalueeditor
{

AbstractAnyValueBuildNode::AbstractAnyValueBuildNode(sup::dto::AnyValue &&value)
    : m_value(std::move(value))
{
}

void AbstractAnyValueBuildNode::Consume(sup::dto::AnyValue &&value)
{
  m_value = std::move(value);
}

sup::dto::AnyValue AbstractAnyValueBuildNode::MoveAnyValue() const
{
  return std::move(m_value);
}

}  // namespace anyvalueeditor
