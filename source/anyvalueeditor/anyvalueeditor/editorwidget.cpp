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

#include "anyvalueeditor/editorwidget.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace anyvalueeditor
{

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent), m_tree_view(new QTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tree_view);
}

}  // namespace anyvalueditor
