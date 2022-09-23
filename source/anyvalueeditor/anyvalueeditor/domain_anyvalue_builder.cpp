/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "anyvalueeditor/domain_anyvalue_builder.h"

#include <anyvalueeditor/anyvalue_build_adapter_v2.h>
#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/scalar_conversion_utils.h>
#include <sup/dto/anyvalue.h>

#include <stack>
#include <cassert>

namespace anyvalueeditor
{

enum NodeContext
{
  kRoot,
  kStructField,
  kArrayElement
};

struct Node
{
  const AnyValueItem* m_item{nullptr};  //!< Current item in the hierarchy.
  std::string m_name;                   //!< The name under which the value is known to its parent.
  bool m_is_visited{false};  //!< True if `value` is a struct and all children are processed.
  NodeContext m_context;

  Node(const AnyValueItem* item, NodeContext context = kRoot) : m_item(item), m_context(context) {}

  bool IsArrayContext() const { return m_context == kArrayElement; }

  bool IsStructContext() const { return m_context == kStructField; }
};

struct DomainAnyValueBuilder::DomainAnyValueBuilderImpl
{
  AnyValueBuildAdapterV2 m_builder;
  std::stack<Node> m_stack;

  explicit DomainAnyValueBuilderImpl(const AnyValueItem& item)
  {
    m_stack.push({&item});
    ProcessItemStack();
  }

  void ProcessItemStack()
  {
    while (!m_stack.empty())
    {
      auto& node = m_stack.top();

      if (node.m_item->IsScalar())
      {
        ProcessScalarItem(node);
      }
      else if (node.m_item->IsStruct())
      {
        ProcessStructNode(node);
      }
      else
      {
        m_stack.pop();
      }
    }
  }

  void ProcessStructNode(Node& node)
  {
    if (!node.m_is_visited)
    {
      ProcessNewStructNode(node);
    }
    else
    {
      ProcessVisitedStructNode(node);
    }
  }

  void ProcessArrayNode(Node& node)
  {
    if (!node.m_is_visited)
    {
      ProcessNewArrayNode(node);
    }
    else
    {
      ProcessVisitedArrayNode(node);
    }
  }

  void ProcessNewStructNode(Node& node)
  {
    StartComposite(node);
    m_builder.StartStruct(node.m_item->GetAnyTypeName());
    AddChildren(node, NodeContext::kStructField);
  }

  void ProcessVisitedStructNode(Node& node)
  {
    m_builder.EndStruct();
    EndComposite(node);
  }

  void ProcessNewArrayNode(Node& node)
  {
    StartComposite(node);
    m_builder.StartArray(node.m_item->GetAnyTypeName());
    AddChildren(node, NodeContext::kArrayElement);
  }

  void ProcessVisitedArrayNode(Node& node)
  {
    m_builder.EndArray();
    EndComposite(node);
  }

  void StartComposite(Node& node)
  {
    assert(node.m_is_visited == false);
    node.m_is_visited = true;

    if (node.IsStructContext())
    {
      m_builder.StartField(node.m_name);
    }
    else if (node.IsArrayContext())
    {
      m_builder.StartArrayElement();
    }

    // this is top level object
  }

  void EndComposite(Node& node)
  {
    assert(node.m_is_visited);
    if (node.IsStructContext())
    {
      m_builder.EndField();
    }
    else if (node.IsArrayContext())
    {
      m_builder.EndArrayElement();
    }
    m_stack.pop();  // we don't need the node anymore
  }

  void AddChildren(Node& node, NodeContext context)
  {
    auto children = node.m_item->GetItems<AnyValueItem>("");
    // iteration in reverse order
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      Node node{const_cast<const AnyValueItem*>(*it), context};
      node.m_name = (*it)->GetDisplayName();
      m_stack.push(node);
    }
  }

  void ProcessScalarItem(Node& node)
  {
    // It's a scalar field. Let's add corresponding field to the AnyValue and remove node from
    // stack. We don't need it anymore.
    if (node.m_name.empty())
    {
      m_builder.AddValue(GetAnyValueFromScalar(*node.m_item));
    }
    else
    {
      m_builder.AddMember(node.m_name, GetAnyValueFromScalar(*node.m_item));
    }

    m_stack.pop();
  }

  //! Process PVXS value representing a struct.
  void ProcessStructItem(Node& node)
  {
    if (node.m_is_visited)
    {
      // All children have been already added to the struct. It's time to tell the builder
      // that the struct has to be added to its own parent.
      m_builder.EndStruct();
      if (!node.m_name.empty())
      {
        m_builder.EndField();
      }
      m_stack.pop();  // we don't need the node anymore
    }
    else
    {
      // We found a struct which we haven't seen before. Let's tell the builder to create
      // underlying AnyValue, and let's add children to the stack.
      // We are not poping struct node, we will get back to it later.
      if (!node.m_name.empty())
      {
        m_builder.StartField(node.m_name);
      }
      m_builder.StartStruct(node.m_item->GetAnyTypeName());
      node.m_is_visited = true;

      auto children = node.m_item->GetItems<AnyValueItem>("");
      // iteration in reverse order
      for (auto it = children.rbegin(); it != children.rend(); ++it)
      {
        Node node{const_cast<const AnyValueItem*>(*it), kStructField};
        node.m_name = (*it)->GetDisplayName();
        m_stack.push(node);
      }
    }
  }

  void AddValueValue(Node& node)
  {
    if (node.IsStructContext())
    {
    }
  }
};

DomainAnyValueBuilder::DomainAnyValueBuilder(const AnyValueItem& item)
    : p_impl(std::make_unique<DomainAnyValueBuilderImpl>(item))
{
}

DomainAnyValueBuilder::~DomainAnyValueBuilder() = default;

sup::dto::AnyValue DomainAnyValueBuilder::GetAnyValue() const
{
  return p_impl->m_builder.MoveAnyValue();
}

}  // namespace anyvalueeditor
