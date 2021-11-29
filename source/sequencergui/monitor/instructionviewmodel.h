/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_INSTRUCTIONVIEWMODEL_H
#define MVVM_VIEWMODEL_INSTRUCTIONVIEWMODEL_H

#include "mvvm/viewmodel/viewmodel.h"

namespace ModelView
{
class ApplicationModel;
}

namespace sequi
{

//! View model to show instruction tree with three columns: type, name, status

class MVVM_VIEWMODEL_EXPORT InstructionViewModel : public ModelView::ViewModel
{
  Q_OBJECT

public:
  InstructionViewModel(ModelView::ApplicationModel* model, QObject* parent = nullptr);
};

}  // namespace sequi

#endif  // MVVM_VIEWMODEL_INSTRUCTIONVIEWMODEL_H
