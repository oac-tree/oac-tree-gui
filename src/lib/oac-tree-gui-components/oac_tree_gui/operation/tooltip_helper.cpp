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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "tooltip_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/core/variant.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_format.h>

#include <QFont>
#include <QTextEdit>

namespace
{

const std::vector<std::string> kSkipAttributeList = {
    oac_tree_gui::itemconstants::kName, oac_tree_gui::domainconstants::kIsRootAttribute,
    oac_tree_gui::itemconstants::kBehaviorTag};

bool IsPropertyToShow(const std::string& tag_name)
{
  return !mvvm::utils::Contains(kSkipAttributeList, tag_name);
}

void AppendTitle(const std::string& text, QTextEdit& text_edit)
{
  text_edit.append("<b>" + QString::fromStdString(text) + "</b>");
}

void AppendDescription(const std::string& text, QTextEdit& text_edit)
{
  if (!text.empty())
  {
    text_edit.append(QString::fromStdString(text));
  }
}

void AppendLittleVerticalGap(QTextEdit& text_edit)
{
  const auto base_font = text_edit.font();
  auto new_font = base_font;
  new_font.setPointSize(static_cast<int>(base_font.pointSize() * 0.3));
  text_edit.setCurrentFont(new_font);
  text_edit.append(" ");
  text_edit.setCurrentFont(base_font);
}

}  // namespace

namespace oac_tree_gui
{

std::vector<std::pair<std::string, std::string>> CollectToolTipAttributes(
    const mvvm::SessionItem* item)
{
  std::vector<std::pair<std::string, std::string>> result;
  for (auto property : mvvm::utils::SinglePropertyItems(*item))
  {
    if (IsPropertyToShow(property->GetTagIndex().GetTag()))
    {
      result.emplace_back(property->GetDisplayName(), mvvm::utils::ValueToString(property->Data()));
    }
  }
  return result;
}

std::string GetAttributeHtml(const std::vector<std::pair<std::string, std::string>>& attributes,
                             int total_width)
{
  static const std::string cell_pattern(R"RAW(
<tr>
<td width="%1">%2</td>
<td width="%3">%4</td>
</tr>
)RAW");

  std::string result;
  if (!attributes.empty())
  {
    result = mvvm::utils::StringFormat("<table width=\"%1\">").arg(std::to_string(total_width));
    for (auto& [name, value] : attributes)
    {
      const int cell_width1 = static_cast<int>(total_width * 0.3);
      const int cell_width2 = static_cast<int>(total_width * 0.7);
      const std::string str = mvvm::utils::StringFormat(cell_pattern)
                                  .arg(std::to_string(cell_width1))
                                  .arg(name)
                                  .arg(std::to_string(cell_width2))
                                  .arg(value);
      result += str;
    }

    result += "</table>";
  }

  return result;
}

QString GetInstructionToolTipText(const mvvm::SessionItem* item)
{
  const auto instruction = mvvm::utils::FindItemUp<InstructionItem>(item);
  if (!instruction)
  {
    return {};
  }

  QTextEdit text_edit;
  const auto base_font = text_edit.font();

  AppendTitle(instruction->GetDomainType(), text_edit);
  AppendDescription(instruction->GetName(), text_edit);
  AppendLittleVerticalGap(text_edit);

  auto attributes = CollectToolTipAttributes(item);
  if (!attributes.empty())
  {
    text_edit.insertHtml(
        QString::fromStdString(GetAttributeHtml(attributes, base_font.pointSize() * 55)));
  }

  return text_edit.toHtml();
}

}  // namespace oac_tree_gui
