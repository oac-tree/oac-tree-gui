/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_
#define SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_

#include <QObject>

namespace sequencergui
{

/**
 * @brief The InstructionTreeSelectionController helper class provides logic for instruction
 * selection on real-time instruction tree.
 *
 * As an input, it uses the instruction currenly being executed. If the instruction is located
 * inside collapsed branch, the algorithm will look for the parent which ownes collapsed branch, and
 * will select it instead. If current instruction is inside expanded branch, will simply select it.
 */
class InstructionTreeSelectionController : public QObject
{
  Q_OBJECT

public:
  explicit InstructionTreeSelectionController(QObject* parent = nullptr);
  ~InstructionTreeSelectionController() override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_
