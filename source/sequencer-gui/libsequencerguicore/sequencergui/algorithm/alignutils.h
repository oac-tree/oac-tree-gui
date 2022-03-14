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

#ifndef SEQUENCERGUI_ALGORITHM_ALIGNUTILS_H
#define SEQUENCERGUI_ALGORITHM_ALIGNUTILS_H

class QPointF;

namespace sequencergui
{
class InstructionItem;
}

namespace sequencergui::algorithm
{

//! Align children of given instruction on graphics scene.  When `force` is false allignment is
//! performed only for instructions with unitialised coordinates.
//! The position of parent `instruction` remains unchanged.
void AlignInstructionTreeWalker(const QPointF& reference, InstructionItem* instruction, bool force);

}  // namespace sequencergui::algorithm

#endif
