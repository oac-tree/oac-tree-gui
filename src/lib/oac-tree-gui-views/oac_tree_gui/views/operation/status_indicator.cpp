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

#include "status_indicator.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/widgets/busy_sign.h>

#include <map>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns style sheet to show QLabel as colored bar.
 */
QString GetColorButtonStyleSheet(const QColor& background_color)
{
  static const QColor border_color(Qt::black);
  static const QString pattern(
      "border-width: 2px; border-radius: 2px; border-color: %1; border-style: solid; background: "
      "%2;");
  return pattern.arg(border_color.name(), background_color.name());
}

/**
 * @brief Returns color for given InstructionStatus.
 */
QColor GetColorForStatus(InstructionStatus status)
{
  // We use the same color for not started and not finished states.
  static const std::map<InstructionStatus, QColor> status_to_color{
      {InstructionStatus::kNotStarted, Qt::lightGray},
      {InstructionStatus::kNotFinished, Qt::yellow},
      {InstructionStatus::kRunning, Qt::yellow},
      {InstructionStatus::kSuccess, Qt::green},
      {InstructionStatus::kFailure, Qt::red},
      {InstructionStatus::kUndefined, Qt::darkGray}};
  auto it = status_to_color.find(status);
  if (it == status_to_color.end())
  {
    throw RuntimeException("Unexpected instruction status: " + ToString(status));
  }

  return it->second;
}

}  // namespace

StatusIndicator::StatusIndicator(QWidget* parent_widget)
    : QLabel(parent_widget), m_busy_sign(new BusySign(this))
{
  setAlignment(Qt::AlignCenter);
  QFont f = font();
  f.setBold(true);
  setFont(f);
  UpdateAppearance();
}

StatusIndicator::~StatusIndicator()
{
  qDebug() << "StatusIndicator destructor";
}

void StatusIndicator::SetInstructionStatus(InstructionStatus status)
{
  if (status != m_current_status)
  {
    m_current_status = status;
    UpdateAppearance();
  }
}

InstructionStatus StatusIndicator::GetInstructionStatus() const
{
  return m_current_status;
}

void StatusIndicator::UpdateAppearance()
{
  static const std::map<InstructionStatus, BusySignType> status_to_indicator_type{
      {InstructionStatus::kNotStarted, BusySignType::kHidden},
      {InstructionStatus::kNotFinished, BusySignType::kAnimated},
      {InstructionStatus::kRunning, BusySignType::kAnimated},
      {InstructionStatus::kSuccess, BusySignType::kHidden},
      {InstructionStatus::kFailure, BusySignType::kHidden},
      {InstructionStatus::kUndefined, BusySignType::kHidden}};

  const auto background_color = GetColorForStatus(m_current_status);
  setStyleSheet(GetColorButtonStyleSheet(background_color));

  m_busy_sign->SetIndicatorType(status_to_indicator_type.at(m_current_status));
}

}  // namespace oac_tree_gui
