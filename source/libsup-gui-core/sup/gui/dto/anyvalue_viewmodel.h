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

#ifndef SUP_GUI_DTO_ANYVALUE_VIEWMODEL_H_
#define SUP_GUI_DTO_ANYVALUE_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class SessionModelInterface;
}

namespace sup::gui
{

//! View model to show AnyValueItem with editable display name.

class AnyValueViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit AnyValueViewModel(mvvm::SessionModelInterface* model, QObject* parent = nullptr);
};

}  // namespace anyvalueeditor

#endif  // SUP_GUI_DTO_ANYVALUE_VIEWMODEL_H_
