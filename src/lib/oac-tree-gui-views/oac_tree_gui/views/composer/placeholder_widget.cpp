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

#include "placeholder_widget.h"

#include <QGridLayout>
#include <QLabel>

namespace oac_tree_gui
{

PlaceholderWidget::PlaceholderWidget(const QString& placeholder_text, QWidget* parent_widget)
    : QWidget(parent_widget), m_label(new QLabel(placeholder_text))
{
  auto layout = new QGridLayout(this);
  layout->addWidget(m_label);
}

}  // namespace oac_tree_gui
