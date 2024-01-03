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

#ifndef SEQUENCERGUI_VIEWMODEL_PROCEDURE_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_PROCEDURE_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class SessionModelInterface;
}

namespace sequencergui
{

//! View model to show ProcedureItem with editable display name.

class MVVM_VIEWMODEL_EXPORT ProcedureViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit ProcedureViewModel(mvvm::SessionModelInterface* model, QObject* parent = nullptr);

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_PROCEDURE_VIEWMODEL_H_
