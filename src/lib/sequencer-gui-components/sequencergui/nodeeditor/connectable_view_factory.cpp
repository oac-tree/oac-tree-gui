/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "connectable_view_factory.h"

#include <sequencergui/mainwindow/app_settings.h>
#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/position_strategy.h>

namespace sequencergui
{

ConnectableViewFactory::ConnectableViewFactory() : m_app_settings(std::make_unique<AppSettings>())
{
}

ConnectableViewFactory::~ConnectableViewFactory() = default;

std::unique_ptr<ConnectableView> ConnectableViewFactory::CreateView(InstructionItem *item)
{
  auto adapter = std::make_unique<ConnectableInstructionAdapter>(item);
  auto result = std::make_unique<ConnectableView>(std::move(adapter));

  if (m_app_settings->IsTopToBottomStyle())
  {
    result->SetPositionStrategy(std::make_unique<TopBottomPositionStrategy>());
  }
  else
  {
    result->SetPositionStrategy(std::make_unique<LeftRightPositionStrategy>());
  }
  return result;
}

}  // namespace sequencergui
