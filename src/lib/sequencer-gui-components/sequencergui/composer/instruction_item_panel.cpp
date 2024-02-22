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

#include "instruction_item_panel.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/widgets/item_list_widget.h>

#include <mvvm/widgets/widget_utils.h>

#include <QVBoxLayout>

namespace sequencergui
{

InstructionItemPanel::InstructionItemPanel(QWidget *parent)
    : QWidget(parent), m_list_widget(new ItemListWidget)
{
  setWindowTitle("INSTRUCTIONS");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_list_widget);

  m_list_widget->AddEntries(::mvvm::utils::GetStringList(GetDomainInstructionNames()));

  connect(m_list_widget, &ItemListWidget::InstructionDoubleClicked, this,
          &InstructionItemPanel::InstructionDoubleClicked);
}

}  // namespace sequencergui
