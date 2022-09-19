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

//! @file anyvalue_buildnode.h
//! Collection of classes representing nodes for AnyValueBuildAdapterV2.

#include <anyvalueeditor/abstract_anyvalue_buildnode.h>
#include <anyvalueeditor/dto_types_fwd.h>

#include <string>

namespace anyvalueeditor
{

//! The node to build AnyValue.

class AnyValueBuildNode : public AbstractAnyValueBuildNode
{
public:
  AnyValueBuildNode(sup::dto::AnyValue&& value);

  NodeType GetNodeType() const override;

  bool Process(std::stack<node_t>& stack) override;
};

//! The node which is created at start of the structure.

class StartStructBuildNode : public AbstractAnyValueBuildNode
{
public:
  StartStructBuildNode(const std::string& struct_name);

  NodeType GetNodeType() const override;

  bool Process(std::stack<node_t>& stack) override;

  bool IsStartStructNode() const override;

  void AddMember(const std::string& name, const sup::dto::AnyValue& value) override;
};

//! The node which is created at start of the structure.

class EndStructBuildNode : public AbstractAnyValueBuildNode
{
public:
  NodeType GetNodeType() const override;

  bool Process(std::stack<node_t>& stack) override;
};

class StartFieldBuildNode : public AbstractAnyValueBuildNode
{
public:
  explicit StartFieldBuildNode(const std::string& field_name);

  NodeType GetNodeType() const override;

  bool Process(std::stack<node_t>& stack) override;

  bool IsStartFieldNode() const override;
};

class EndFieldBuildNode : public AbstractAnyValueBuildNode
{
public:
  NodeType GetNodeType() const override;

  bool Process(std::stack<node_t>& stack) override;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILDNODES_H_
