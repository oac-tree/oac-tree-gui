/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "align_node.h"

#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace sequencergui::algorithm
{

int AlignNode::GetSize() const
{
  return static_cast<int>(m_children.size());
}

AlignNode *AlignNode::GetParent() const
{
  return m_parent;
}

void AlignNode::SetParent(AlignNode *parent)
{
  m_parent = parent;
}

std::vector<AlignNode *> AlignNode::GetChildren() const
{
  std::vector<AlignNode *> result;
  (void)std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                       [](const auto &x) { return x.get(); });
  return result;
}

AlignNode *AlignNode::Add(std::unique_ptr<AlignNode> child)
{
  auto result = child.get();
  (void)m_children.emplace_back(std::move(child));
  result->SetParent(this);
  return result;
}

double AlignNode::GetX() const
{
  return m_xpos;
}

void AlignNode::SetX(double value)
{
  m_xpos = value;
}

double AlignNode::GetY() const
{
  return m_ypos;
}

void AlignNode::SetY(double value)
{
  m_ypos = value;
}

double AlignNode::GetMod() const
{
  return m_mod;
}

void AlignNode::SetMod(double value)
{
  m_mod = value;
}

void AlignNode::SetPos(double x, double y)
{
  SetX(x);
  SetY(y);
}

bool AlignNode::IsLeaf() const
{
  return GetSize() == 0;
}

bool AlignNode::IsLeftMost() const
{
  return GetParent() ? (GetParent()->GetChildren().front() == this) : true;
}

bool AlignNode::IsRightMost() const
{
  return GetParent() ? (GetParent()->GetChildren().back() == this) : true;
}

AlignNode *AlignNode::GetPreviousSibling() const
{
  if (!GetParent() || IsLeftMost())
  {
    return nullptr;
  }

  auto children = GetParent()->GetChildren();
  auto index = mvvm::utils::IndexOfItem(children, this);
  return children.at(static_cast<std::size_t>(index - 1));
}

AlignNode *AlignNode::GetNextSibling() const
{
  if (!GetParent() || IsRightMost())
  {
    return nullptr;
  }

  auto children = GetParent()->GetChildren();
  auto index = mvvm::utils::IndexOfItem(children, this);
  return children.at(static_cast<std::size_t>(index + 1));
}

AlignNode *AlignNode::GetLeftMostSibling()
{
  if (!GetParent())
  {
    return nullptr;
  }

  if (IsLeftMost())
  {
    return this;
  }

  return GetParent()->GetChildren().at(0);
}

AlignNode *AlignNode::GetLeftMostChild() const
{
  return GetSize() == 0 ? nullptr : GetChildren().front();
}

AlignNode *AlignNode::GetRightMostChild() const
{
  return GetSize() == 0 ? nullptr : GetChildren().back();
}

double AlignNode::GetNodeSize() const
{
  const double arbitrary_node_size = 1.0;
  return arbitrary_node_size;
}

double AlignNode::GetSiblingDistance() const
{
  const double arbitrary_sibling_distance = 0.0;
  return arbitrary_sibling_distance;
}

std::string AlignNode::GetIdentifier() const
{
  return m_identifier;
}

void AlignNode::SetIdentifier(const std::string &identifier)
{
  m_identifier = identifier;
}

//! Returns index of given item in the array of its parent
int AlignNode::GetIndex() const
{
  return GetParent() ? mvvm::utils::IndexOfItem(GetParent()->GetChildren(), this) : -1;
}

}  // namespace sequencergui::algorithm
