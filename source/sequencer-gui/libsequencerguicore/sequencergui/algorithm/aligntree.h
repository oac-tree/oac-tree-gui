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

#ifndef SEQUENCERGUI_ALGORITHM_ALIGNTREE_H
#define SEQUENCERGUI_ALGORITHM_ALIGNTREE_H

#include <memory>

namespace sequencergui::algorithm
{

class AlignNode;

//! The tree of AlignNode's.
//! Part of the machinery for general tree alignment.

class AlignTree
{
public:
  AlignTree();

  AlignNode* GetRootNode();

  AlignNode* Add(AlignNode* parent);

private:
  std::unique_ptr<AlignNode> m_root_node;
};

}  // namespace sequencergui::algorithm

#endif // SEQUENCERGUI_ALGORITHM_ALIGNTREE_H

