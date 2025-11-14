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

#include "splittable_widget.h"

#include <oac_tree_gui/core/exceptions.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace oac_tree_gui
{

SplittableWidget::SplittableWidget(CreateWidgetCallback callback, QWidget* parent_widget)
    : QWidget(parent_widget), m_callback(std::move(callback)), m_splitter(new QSplitter)
{
  if (!m_callback)
  {
    throw RuntimeException("Callback is not defined");
  }

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  auto initial_widget = m_callback();
  m_splitter->addWidget(initial_widget.release());
}

SplittableWidget::~SplittableWidget() = default;

QSplitter* SplittableWidget::GetSplitter() const
{
  return m_splitter;
}

void SplittableWidget::AddWidget(QWidget* after_widget)
{
  auto new_widget = m_callback();
  if (after_widget == nullptr)
  {
    m_splitter->addWidget(new_widget.release());
  }
  else
  {
    const std::int32_t index = m_splitter->indexOf(after_widget);
    m_splitter->insertWidget(index + 1, new_widget.release());
  }
}

void SplittableWidget::CloseWidget(QWidget* widget_to_close)
{
  widget_to_close->hide();
  widget_to_close->deleteLater();
}

}  // namespace oac_tree_gui
