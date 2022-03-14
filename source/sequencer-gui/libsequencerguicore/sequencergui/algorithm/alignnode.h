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

#ifndef SEQUENCERGUI_ALGORITHM_ALIGNNODE_H
#define SEQUENCERGUI_ALGORITHM_ALIGNNODE_H

#include <memory>
#include <vector>

namespace sequencergui::algorithm
{

//! The node of the AlignTree.
//! Part of the machinery for general tree alignment.

class AlignNode
{
public:
  int GetSize() const;

  AlignNode* GetParent() const;
  void SetParent(AlignNode* parent);

  std::vector<AlignNode*> GetChildren() const;

  AlignNode* Add(std::unique_ptr<AlignNode> child);

  double GetX() const;
  void SetX(double value);

  double GetY() const;
  void SetY(double value);

private:
  AlignNode* m_parent{nullptr};
  std::vector<std::unique_ptr<AlignNode>> m_children;
  double m_xpos{0.0};
  double m_ypos{0.0};
};

}  // namespace sequencergui::algorithm

#endif  // SEQUENCERGUI_ALGORITHM_ALIGNNODE_H
