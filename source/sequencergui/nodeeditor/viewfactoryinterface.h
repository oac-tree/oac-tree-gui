/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H
#define SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H

#include <memory>

namespace sequi
{

class InstructionItem;
class ConnectableView;

class ViewFactoryInterface
{
public:
  virtual std::unique_ptr<ConnectableView> CreateView(InstructionItem*) = 0;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H
