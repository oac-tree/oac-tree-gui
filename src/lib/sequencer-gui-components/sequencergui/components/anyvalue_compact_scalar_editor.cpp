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

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QHBoxLayout>
#include <QLabel>

namespace sequencergui
{

AnyValueCompactScalarEditor::AnyValueCompactScalarEditor(QWidget *parent)
    : AbstractAnyValueEditor(parent)
{
}

void AnyValueCompactScalarEditor::SetInitialValue(const sup::gui::AnyValueItem *item) {}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueCompactScalarEditor::GetResult()
{
  return {};
}

AnyValueCompactScalarEditor::~AnyValueCompactScalarEditor() = default;

}  // namespace sequencergui
