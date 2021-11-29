/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableviewfactory.h"

#include "sequencergui/mainwindow/appsettings.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/positionstrategy.h"

namespace sequi
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

}  // namespace sequi
