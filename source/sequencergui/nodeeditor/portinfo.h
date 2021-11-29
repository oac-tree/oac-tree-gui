/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_PORTINFO_H
#define SEQUENCERGUI_NODEEDITOR_PORTINFO_H

#include <string>

namespace sequi
{
//! Aggregate to hold basic port information (i.e. type and color) for NodePort class.
//! Colors must be what QColor understands (i.e. https://www.w3.org/TR/css-color-3/#svg-color).

struct PortInfo
{
  const std::string m_type;
  const std::string m_color;
};

const static PortInfo kBasicParentPortInfo = {"BasicParent", "cornflowerblue"};
const static PortInfo kBasicChildPortInfo = {"BasicChild", "navajowhite"};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_PORTINFO_H
