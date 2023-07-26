/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class SessionModelInterface;
}

namespace sequencergui
{

//! View model to show instruction tree with three columns: display_name, name and status.

class MVVM_VIEWMODEL_EXPORT InstructionEditorViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit InstructionEditorViewModel(mvvm::SessionModelInterface* model, QObject* parent = nullptr);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_INSTRUCTION_VIEWMODEL_H_
