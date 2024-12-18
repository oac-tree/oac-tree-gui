/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPONENTS_STYLE_HELPER_H_
#define SEQUENCERGUI_COMPONENTS_STYLE_HELPER_H_

#include <sup/gui/style/icon_color_flavor.h>

class QIcon;
class QString;

namespace sequencergui
{

/**
 * @brief Finds an icon with the given name and dark/light color flavor.
 *
 * Example: FindIcon("animation", AppIconColorFlavor::kForDarkThemes)
 * Is assumes an existence of animation.svg file in the resource folder of sequencer package.
 */
QIcon FindIcon(const QString& icon_name,
               sup::gui::IconColorFlavor icon_flavor = sup::gui::IconColorFlavor::kUnspecified);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_STYLE_HELPER_H_
