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

#include "anyvalue_compact_scalar_editor.h"

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QHBoxLayout>
#include <QLabel>

namespace sequencergui
{

AnyValueCompactScalarEditor::AnyValueCompactScalarEditor(QWidget *parent)
    : QWidget(parent), m_label(new QLabel)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_label);
}

AnyValueCompactScalarEditor::~AnyValueCompactScalarEditor() = default;

void AnyValueCompactScalarEditor::SetDescription(const QString &text)
{
  m_label->setText(text);
}

}  // namespace sequencergui
