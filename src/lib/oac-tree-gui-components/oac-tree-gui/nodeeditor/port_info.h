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

#ifndef SEQUENCERGUI_NODEEDITOR_PORT_INFO_H_
#define SEQUENCERGUI_NODEEDITOR_PORT_INFO_H_

#include <string>

namespace oac_tree_gui
{
//! Aggregate to hold basic port information (i.e. type and color) for NodePort class.
//! Colors must be what QColor understands (i.e. https://www.w3.org/TR/css-color-3/#svg-color).

struct PortInfo
{
  std::string m_type;
  std::string m_color;
};

const static PortInfo kBasicPortInfo = {"BasicPort", "navajowhite"};
const static PortInfo kRootPortInfo = {"BasicPort", "indianred"};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_NODEEDITOR_PORT_INFO_H_
