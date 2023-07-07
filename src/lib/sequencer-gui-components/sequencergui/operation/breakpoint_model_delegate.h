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

#ifndef SEQUENCERGUI_OPERATION_BREAKPOINT_MODEL_DELEGATE_H_
#define SEQUENCERGUI_OPERATION_BREAKPOINT_MODEL_DELEGATE_H_

#include <QStyledItemDelegate>

namespace sequencergui
{

/**
 * @brief The BreakpointModelDelegate class is a delagate for InstructionViewModel with the task to
 * paint breakpoints in a 3rd row.
 */
class BreakpointModelDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit BreakpointModelDelegate(QObject *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_MODEL_DELEGATE_H_
