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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H
#define SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace sequi
{
class InstructionListWidget;

//! Collapsible list with instructions available to drag and drop on graphics scene.

class InstructionPanel : public CollapsibleWidget
{
  Q_OBJECT

public:
  InstructionPanel(QWidget* parent = nullptr);

private:
  InstructionListWidget* m_list_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H
