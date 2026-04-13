/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_attribute_editor_widget.h"

#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/views/property_tree_view.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

ProcedureAttributeEditorWidget::ProcedureAttributeEditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_property_tree_view(new mvvm::PropertyTreeView)
{
  setWindowTitle("Procedure Attributes");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_property_tree_view);
}

ProcedureAttributeEditorWidget::~ProcedureAttributeEditorWidget() = default;

void ProcedureAttributeEditorWidget::SetProcedure(ProcedureItem* procedure_item)
{
  m_property_tree_view->SetItem(procedure_item);
}

}  // namespace oac_tree_gui
