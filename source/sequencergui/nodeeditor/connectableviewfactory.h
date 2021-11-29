/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWFACTORY_H
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWFACTORY_H

#include "sequencergui/nodeeditor/viewfactoryinterface.h"

#include <memory>

namespace sequi
{

class AppSettings;

//!Factory to construct ConnectableViews from corre

class ConnectableViewFactory : public ViewFactoryInterface
{
public:
  ConnectableViewFactory();
  ~ConnectableViewFactory();

  std::unique_ptr<ConnectableView> CreateView(InstructionItem *item) override;

private:
  std::unique_ptr<AppSettings> m_app_settings;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWFACTORY_H
