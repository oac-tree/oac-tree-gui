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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H_
#define OAC_TREE_GUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H_

#include <QProxyStyle>

namespace oac_tree_gui
{

/**
 * @brief The CustomTreeViewStyle class provides drag-and-drop indicator that occupies whole
 * horizontal space of the tree.
 *
 * @details From https://stackoverflow.com/questions/7596584/qtreeview-draw-drop-indicator
 */

class CustomTreeViewStyle : public QProxyStyle
{
public:
  explicit CustomTreeViewStyle(QStyle* style);

  void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget) const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H_
