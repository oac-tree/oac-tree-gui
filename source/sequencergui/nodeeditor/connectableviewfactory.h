/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
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
