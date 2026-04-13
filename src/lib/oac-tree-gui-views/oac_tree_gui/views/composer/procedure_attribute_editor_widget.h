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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_ATTRIBUTE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_ATTRIBUTE_EDITOR_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class PropertyTreeView;
}

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The ProcedureAttributeEditorWidget is a simple attribute editor for procedure.
 *
 * To modify the name of the procedure, list of plugins, JSON types.
 */
class ProcedureAttributeEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureAttributeEditorWidget(QWidget* parent_widget = nullptr);
  ~ProcedureAttributeEditorWidget() override;

  ProcedureAttributeEditorWidget(const ProcedureAttributeEditorWidget&) = delete;
  ProcedureAttributeEditorWidget& operator=(const ProcedureAttributeEditorWidget&) = delete;
  ProcedureAttributeEditorWidget(ProcedureAttributeEditorWidget&&) noexcept = delete;
  ProcedureAttributeEditorWidget& operator=(ProcedureAttributeEditorWidget&&) noexcept = delete;

  void SetProcedure(ProcedureItem* procedure_item);

private:
  mvvm::PropertyTreeView* m_property_tree_view{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_ATTRIBUTE_EDITOR_WIDGET_H_
