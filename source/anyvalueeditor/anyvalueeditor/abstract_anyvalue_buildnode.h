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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_I_ANYVALUE_BUILDNODE_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_I_ANYVALUE_BUILDNODE_H_

#include <sup/dto/anyvalue.h>

#include <memory>
#include <stack>
#include <string>

namespace anyvalueeditor
{

//! The node for AnyValueBuildAdapter to build AnyValue.

class AbstractAnyValueBuildNode
{
public:
  using node_t = std::unique_ptr<AbstractAnyValueBuildNode>;

  enum class NodeType
  {
    kValue,
    kStartStruct,
    kEndStruct,
    kStartField,
    kEndField,
    kStartArray,
    kEndArray
  };

  AbstractAnyValueBuildNode() = default;
  AbstractAnyValueBuildNode(sup::dto::AnyValue&& value);

  virtual NodeType GetNodeType() const = 0;

  virtual bool Process(std::stack<node_t>& stack) = 0;

  virtual void Consume(sup::dto::AnyValue&& value);

  sup::dto::AnyValue MoveAnyValue() const;

  virtual bool IsStartStructNode() const;

  virtual bool IsStartElementNode() const;

  virtual bool IsStartFieldNode() const;

  static bool CanAddValueNode(const std::stack<node_t>& stack);

  std::string GetFieldName() const;
  void SetFieldName(const std::string& name);

private:
  sup::dto::AnyValue m_value;
  std::string m_field_name;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_I_ANYVALUE_BUILDNODE_H_
