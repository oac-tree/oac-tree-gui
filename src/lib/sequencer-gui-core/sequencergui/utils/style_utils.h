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

#ifndef SEQUENCERGUI_UTILS_STYLE_UTILS_H_
#define SEQUENCERGUI_UTILS_STYLE_UTILS_H_

#include <QWidget>

class QIcon;
class QTreeView;

namespace sequencergui::styleutils
{

//! Returns characteristic UI size scaled by a given factor.
//! Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's
//! own virtual coordinate system. The user is expected to express all widget sizes in these units.
int UnitSize(double scale = 1.0);

//! Returns default point size of the application's font.
int AppFontSize();

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();

QIcon GetIcon(const std::string& icon_name);

//! Sets style for the tree to better look on CODAC systems.
void SetUnifiedPropertyStyle(QTreeView* tree);

}  // namespace sequencergui::styleutils

#endif  // SEQUENCERGUI_UTILS_STYLE_UTILS_H_
