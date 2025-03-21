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

#include "tooltip_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/core/variant.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

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
  new_font.setPointSize(static_cast<int>(base_font.pointSize() * 0.25));
  text_edit.setCurrentFont(new_font);
  text_edit.append(" ");
  text_edit.setCurrentFont(base_font);
}

void AppendNameValuePair(const std::string& s_name, const std::string& s_value,
                         QTextEdit& text_edit)
{
  static const QString pattern(R"RAW(<div style="width=%1px"><pre>%2:%3</pre></div>)RAW");
  const int name_field_length{18};
  const auto base_font = text_edit.font();

  const QFont f("Monospace", base_font.pointSize());
  text_edit.setCurrentFont(f);

  QString name = QString::fromStdString(s_name);
  name.resize(name_field_length, ' ');
  const QString value = QString::fromStdString(s_value);

  text_edit.append(pattern.arg(base_font.pointSize() * 25).arg(name).arg(value));
}

}  // namespace

namespace oac_tree_gui
{

QString GetInstructionToolTipText(const mvvm::SessionItem* item)
{
  const auto instruction = mvvm::utils::FindItemUp<InstructionItem>(item);
  if (!instruction)
  {
    return {};
  }

  QTextEdit text_edit;

  AppendTitle(instruction->GetDomainType(), text_edit);
  AppendDescription(instruction->GetName(), text_edit);
  AppendLittleVerticalGap(text_edit);

  for (auto property : mvvm::utils::SinglePropertyItems(*instruction))
  {
    if (IsPropertyToShow(property->GetTagIndex().GetTag()))
    {
      AppendNameValuePair(property->GetDisplayName(), mvvm::utils::ValueToString(property->Data()),
                          text_edit);
    }
  }

  return text_edit.toHtml();
}

}  // namespace oac_tree_gui
