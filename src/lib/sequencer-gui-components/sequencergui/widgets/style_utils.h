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

#ifndef SEQUENCERGUI_WIDGETS_STYLE_UTILS_H_
#define SEQUENCERGUI_WIDGETS_STYLE_UTILS_H_

#include <QWidget>

class QTreeView;
class QIcon;

namespace sequencergui::styleutils
{

//! Sets style for the tree to better look on CODAC systems.
void SetUnifiedPropertyStyle(QTreeView* tree);

QIcon GetIcon(const std::string& icon_name);

}  // namespace sequencergui::styleutils

#endif  // SEQUENCERGUI_WIDGETS_STYLE_UTILS_H_
