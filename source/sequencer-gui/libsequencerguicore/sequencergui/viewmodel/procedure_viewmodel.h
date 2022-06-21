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

#ifndef SEQUENCERGUI_MONITOR_PROCEDURE_VIEWMODEL_H
#define SEQUENCERGUI_MONITOR_PROCEDURE_VIEWMODEL_H

#include "mvvm/viewmodel/viewmodel.h"

namespace mvvm
{
class ApplicationModel;
}

namespace sequencergui
{

//! View model to show ProcedureItem with editable display name.

class MVVM_VIEWMODEL_EXPORT ProcedureViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit ProcedureViewModel(mvvm::ApplicationModel* model, QObject* parent = nullptr);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_PROCEDURE_VIEWMODEL_H
