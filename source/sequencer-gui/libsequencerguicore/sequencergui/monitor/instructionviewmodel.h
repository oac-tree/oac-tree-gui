/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#ifndef MVVM_VIEWMODEL_INSTRUCTIONVIEWMODEL_H
#define MVVM_VIEWMODEL_INSTRUCTIONVIEWMODEL_H

#include "mvvm/viewmodel/viewmodel.h"

namespace ModelView
{
class ApplicationModel;
}

namespace sequencergui
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
