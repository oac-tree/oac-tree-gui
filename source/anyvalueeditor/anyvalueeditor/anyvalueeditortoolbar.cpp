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

#include "anyvalueeditor/anyvalueeditortoolbar.h"

#include "mvvm/widgets/widgetutils.h"

#include <QPushButton>
#include <QToolButton>

namespace anyvalueeditor
{
AnyValueEditorToolBar::AnyValueEditorToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_insert_after_button(new QToolButton)
    , m_insert_into_button(new QToolButton)
    , m_remove_button(new QToolButton)
{
  setIconSize(QSize(24, 24));

  m_insert_after_button->setText("Insert after");
  m_insert_after_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  addWidget(m_insert_after_button);

  m_insert_into_button->setText("Insert into");
  m_insert_into_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  addWidget(m_insert_into_button);

  m_remove_button->setText("Remove");
  m_remove_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  connect(m_remove_button, &QToolButton::clicked, this,
          &AnyValueEditorToolBar::removeSelectedRequest);
  addWidget(m_remove_button);
}

AnyValueEditorToolBar::~AnyValueEditorToolBar() = default;

void AnyValueEditorToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace anyvalueeditor
