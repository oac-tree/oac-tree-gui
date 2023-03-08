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

#include <mvvm/widgets/widget_utils.h>

#include <QIcon>
#include <QSize>
#include <cmath>

namespace sup::gui::utils
{

QSize ToolBarIconSize()
{
  const int width = mvvm::utils::UnitSize(2.3);
  return {width, width};
}

QSize NarrowToolBarIconSize()
{
  const int width = std::round(mvvm::utils::UnitSize(1.75));
  return {width, width};
}

}  // namespace sup::gui::utils
