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
#include <sequencergui/viewmodel/instruction_toolkit_viewmodel.h>
#include <sequencergui/widgets/item_list_widget.h>

#include <mvvm/widgets/widget_utils.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

InstructionItemPanel::InstructionItemPanel(QWidget *parent)
    : QWidget(parent)
    , m_instruction_toolkit_viewmodel(new InstructionToolKitViewModel(this))
    , m_list_widget(new ItemListWidget)
    , m_tree_view(new QTreeView)
{
  setWindowTitle("INSTRUCTIONS");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_list_widget);
  layout->addWidget(m_tree_view);

  m_list_widget->AddEntries(::mvvm::utils::GetStringList(GetDomainInstructionNames()));

  connect(m_list_widget, &ItemListWidget::InstructionDoubleClicked, this,
          &InstructionItemPanel::InstructionDoubleClicked);

  m_tree_view->setModel(m_instruction_toolkit_viewmodel);
}

}  // namespace sequencergui
