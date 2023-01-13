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

#include "sequencergui/widgets/overlay_message.h"

#include "sequencergui/widgets/overlay_message_frame.h"

#include <sequencergui/widgets/overlay_widget_position_strategy.h>
#include <sequencergui/widgets/warning_sign_widget.h>

#include <QAbstractScrollArea>

namespace sequencergui
{

OverlayMessage::~OverlayMessage() = default;

OverlayMessage::OverlayMessage(const QString &text, QWidget *area)
{
  //    m_message_widget = std::make_unique<OverlayMessageFrame>(text, area);
  m_message_widget = std::make_unique<WarningSignWidget>(text, area);
  m_position_strategy =
      std::make_unique<OverlayWidgetPositionStrategy>(m_message_widget.get(), area);
}

bool OverlayMessage::CanBeDeleted()
{
  if (auto widget = dynamic_cast<WarningSignWidget *>(m_message_widget.get()); widget)
  {
    return !widget->IsBusy();
  }
  return true;
}

}  // namespace sequencergui
