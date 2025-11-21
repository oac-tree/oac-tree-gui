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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_EDITOR_PLACEHOLDER_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_EDITOR_PLACEHOLDER_H_

#include <QWidget>

class QLabel;

namespace oac_tree_gui
{

/**
 * @brief The ProcedureEditorPlaceholder class shows a text in the middle of the screen.
 *
 * It is intended to substitute some other widget, when it is not ready yet.
 */
class ProcedureEditorPlaceholder : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureEditorPlaceholder(QWidget* parent_widget = nullptr);

private:
  QLabel* m_label{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_EDITOR_PLACEHOLDER_H_
