/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_HELPER_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_HELPER_H_

//! @file
//! Helper functions for StatusIndicator.

#include <oac_tree_gui/model/instruction_status.h>
#include <oac_tree_gui/widgets/busy_sign.h>

#include <QColor>
#include <QString>

namespace oac_tree_gui
{

/**
 * @brief Returns BusySign type corresponding to a given instruction status.
 */
BusySignType GetBusySignTypeForInstructionStatus(InstructionStatus status);

/**
 * @brief Returns color corresponding to a given instruction status.
 */
QColor GetColorForInstructionStatus(InstructionStatus status);

/**
 * @brief Returns text label corresponding to a given instruction status.
 */
QString GetLabelTextForInstructionStatus(InstructionStatus status);

}  // namespace oac_tree_gui

#endif  //  OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_HELPER_H_
