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

#include "sequencergui/algorithm/alignnode.h"

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
  std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                 [](const auto &x) { return x.get(); });
  return result;
}

AlignNode *AlignNode::Add(std::unique_ptr<AlignNode> child)
{
  auto result = child.get();
  m_children.emplace_back(std::move(child));
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

}  // namespace sequencergui::algorithm
