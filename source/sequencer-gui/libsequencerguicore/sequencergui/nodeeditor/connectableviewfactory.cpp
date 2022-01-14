/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableviewfactory.h"

#include "sequencergui/mainwindow/appsettings.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/positionstrategy.h"

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
