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

#ifndef SEQUENCERGUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H
#define SEQUENCERGUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H

#include <QProxyStyle>

namespace sequencergui
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_CUSTOM_TREE_VIEW_STYLE_H
