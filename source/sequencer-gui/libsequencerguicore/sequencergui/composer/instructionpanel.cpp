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

#include "sequencergui/composer/instructionpanel.h"

#include "sequencergui/nodeeditor/instructionlistwidget.h"

#include <QVBoxLayout>

namespace sequencergui
{


InstructionPanel::InstructionPanel(QWidget *parent)
    : CollapsibleWidget(parent), m_list_widget(new InstructionListWidget)
{
  SetText("INSTRUCTIONS");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_widget);


}

}  // namespace sequi
