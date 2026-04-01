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

#include "status_indicator_widget.h"

#include "status_indicator.h"

#include <QHBoxLayout>

namespace oac_tree_gui
{

StatusIndicatorWidget::StatusIndicatorWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_status_indicator(new StatusIndicator(this))
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_status_indicator);
  // setFixedSize(30, 30);
}

void StatusIndicatorWidget::SetInstructionStatus(InstructionStatus status)
{
  m_status_indicator->SetInstructionStatus(status);
}

}  // namespace oac_tree_gui
