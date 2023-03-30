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

#include "style_utils.h"

#include <mvvm/widgets/style_utils.h>

#include <QApplication>
#include <QStyle>

namespace
{
//! Flag if to use svg versions of icons.
const bool kSvgIcons = false;
}  // namespace

namespace sequencergui::styleutils
{

void SetUnifiedPropertyStyle(QTreeView *tree)
{
  // Sets "breeze" property style only if current theme is a "fusion", which is the default theme
  // on CODAC systems.
  if (QApplication::style()->objectName() == QString("fusion"))
  {
    mvvm::utils::SetBreezePropertyStyle(tree);
  }
}

QIcon GetIcon(const std::string &icon_name)
{
  auto name = QString(":/icons/%1").arg(QString::fromStdString(icon_name));
  if (!kSvgIcons)
  {
    name.replace(".svg", ".png");
  }
  return QIcon(name);
}

}  // namespace sequencergui::styleutils
