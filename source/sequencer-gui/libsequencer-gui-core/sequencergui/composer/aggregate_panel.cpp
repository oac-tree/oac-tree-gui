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

#include "sequencergui/composer/aggregate_panel.h"

#include <mvvm/widgets/widget_utils.h>
#include <sequencergui/model/aggregate_factory.h>
#include <sequencergui/widgets/item_list_widget.h>

#include <QListWidget>
#include <QVBoxLayout>

namespace sequencergui
{

AggregatePanel::AggregatePanel(QWidget *parent) : QWidget(parent), m_list_widget(new ItemListWidget)
{
  setWindowTitle("AGGREGATES");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_list_widget);

  AggregateFactory factory;
  m_list_widget->AddEntries(::mvvm::utils::GetStringList(factory.GetKeys()));
}

}  // namespace sequencergui
