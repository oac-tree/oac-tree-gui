/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "procedure_editor_placeholder.h"

#include <mvvm/widgets/widget_utils.h>

#include <QGridLayout>
#include <QLabel>

namespace oac_tree_gui
{

namespace
{

constexpr auto kHeader = R"(
<html><body style="color:#909090; font-size:14px">
<div align='center'>
<div style="font-size:18px">Create new or open existing procedure</div>
<table><tr><td>
)";

constexpr auto kFooter = R"(
</td></tr></table>
</div>
</body></html>
)";

constexpr auto kDiv1 = R"(
<div style="margin-top: 12px">%1</div>
)";

QString CreatePlaceholderText()
{
  const QString header(kHeader);
  auto clickable_text = mvvm::utils::ClickableText("+ create new ", "new");
  const QString create_new_text = QString(kDiv1).arg(clickable_text);
  const QString footer(kFooter);
  // return header + create_new_text + footer;
  return header + footer;
}

}  // namespace

ProcedureEditorPlaceholder::ProcedureEditorPlaceholder(QWidget* parent_widget)
    : QWidget(parent_widget), m_label(new QLabel(CreatePlaceholderText()))
{
  auto layout = new QGridLayout(this);
  layout->addWidget(m_label);

  connect(m_label, &QLabel::linkActivated, this, [this](const auto& link) { OnLabelClick(link); });
}

void ProcedureEditorPlaceholder::OnLabelClick(const QString& link)
{
  Q_UNUSED(link)
  emit createNewProcedureRequest();
}

}  // namespace oac_tree_gui
