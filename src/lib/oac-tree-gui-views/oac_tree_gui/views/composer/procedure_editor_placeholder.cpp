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

#include <QGridLayout>
#include <QLabel>

namespace oac_tree_gui
{

namespace
{
const QString placeholderText = QString(
    "<html><body style=\"color:#909090; font-size:14px\">"
    "<div align='center'>"
    "<div style=\"font-size:20px\">Open a procedure</div>"
    "<table><tr><td>"
    "<hr/>"
    "<div style=\"margin-top: 5px\">&bull; File > Open File or Project (%1)</div>"
    "<div style=\"margin-top: 5px\">&bull; File > Recent Files</div>"
    "<div style=\"margin-top: 5px\">&bull; Tools > Locate (%2) and</div>"
    "<div style=\"margin-left: 1em\">- type to open file from any open project</div>"
    "%4"
    "%5"
    "<div style=\"margin-left: 1em\">- type <code>%3&lt;space&gt;&lt;filename&gt;</code> to open "
    "file from file system</div>"
    "<div style=\"margin-left: 1em\">- select one of the other filters for jumping to a "
    "location</div>"
    "<div style=\"margin-top: 5px\">&bull; Drag and drop files here</div>"
    "</td></tr></table>"
    "</div>"
    "</body></html>");
}

ProcedureEditorPlaceholder::ProcedureEditorPlaceholder(QWidget* parent_widget)
    : QWidget(parent_widget), m_label(new QLabel(placeholderText))
{
  auto layout = new QGridLayout(this);
  layout->addWidget(m_label);
}

}  // namespace oac_tree_gui
