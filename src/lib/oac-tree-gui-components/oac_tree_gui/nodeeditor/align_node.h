/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_NODEEDITOR_ALIGN_NODE_H_
#define OAC_TREE_GUI_NODEEDITOR_ALIGN_NODE_H_

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace oac_tree_gui::algorithm
{

//! The node of the generic tree. Carries the information necessary for the alignment algorithm to
//! function.

class AlignNode
{
public:
  int GetSize() const;

  AlignNode* GetParent() const;
  void SetParent(AlignNode* parent);

  std::vector<AlignNode*> GetChildren() const;

  AlignNode* Add(std::unique_ptr<AlignNode> child);

  template <typename T>
  T* Add();

  double GetX() const;
  void SetX(double value);

  double GetY() const;
  void SetY(double value);

  double GetMod() const;
  void SetMod(double value);

  void SetPos(double x, double y);

  bool IsLeaf() const;

  bool IsLeftMost() const;

  bool IsRightMost() const;

  AlignNode* GetPreviousSibling() const;

  AlignNode* GetNextSibling() const;

  AlignNode* GetLeftMostSibling();

  AlignNode* GetLeftMostChild() const;

  AlignNode* GetRightMostChild() const;

  double GetNodeSize() const;

  double GetSiblingDistance() const;

  std::string GetIdentifier() const;
  void SetIdentifier(const std::string& identifier);

  std::int32_t GetIndex() const;

private:
  AlignNode* m_parent{nullptr};
  std::vector<std::unique_ptr<AlignNode>> m_children;
  double m_xpos{0.0};
  double m_ypos{0.0};
  double m_mod{0.0};
  std::string m_identifier;
};

template <typename T>
inline T* AlignNode::Add()
{
  return Add(std::make_unique<T>());
}

}  // namespace oac_tree_gui::algorithm

#endif  // OAC_TREE_GUI_NODEEDITOR_ALIGN_NODE_H_
