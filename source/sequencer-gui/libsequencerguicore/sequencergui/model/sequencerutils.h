/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_SEQUENCERUTILS_H
#define SEQUENCERGUI_MODEL_SEQUENCERUTILS_H

//! @file sequencerutils.h
//! Various utils for GUI items

namespace sequencergui
{

class InstructionItem;
class SequencerModel;

bool IsCompoundInstruction(const InstructionItem* instruction);

bool IsDecoratorInstruction(const InstructionItem* instruction);

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_SEQUENCERUTILS_H
