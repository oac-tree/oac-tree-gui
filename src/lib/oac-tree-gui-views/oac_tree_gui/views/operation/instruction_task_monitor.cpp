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

#include "instruction_task_monitor.h"

#include "instruction_task_area_widget.h"

#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskMonitor::InstructionTaskMonitor(QWidget* parent_widget)
    : QWidget(parent_widget), m_task_area_widget(new InstructionTaskAreaWidget(nullptr))
{
  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
}

}  // namespace oac_tree_gui
