/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>

#include <mvvm/core/variant.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <QFont>
#include <QTextEdit>

namespace
{
const std::vector<std::string> kSkipDomainAttributeList = {
    sequencergui::itemconstants::kName, sequencergui::domainconstants::kIsRootAttribute};

bool IsPropertyToShow(const std::string& tag_name)
{
  return !mvvm::utils::Contains(kSkipDomainAttributeList, tag_name);
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
  auto base_font = text_edit.font();
  auto new_font = base_font;
  new_font.setPointSize(5);
  text_edit.setCurrentFont(new_font);
  text_edit.append(" ");
  text_edit.setCurrentFont(base_font);
}

void AppendNameValuePair(const std::string& s_name, const std::string& s_value,
                         QTextEdit& text_edit)
{
  auto base_font = text_edit.font();

  QFont f("Monospace", base_font.pointSize());
  text_edit.setCurrentFont(f);

  QString name = QString::fromStdString(s_name);
  name.resize(18, ' ');
  QString value = QString::fromStdString(s_value);

  text_edit.append("<div style=\"width=500px\"><pre>" + name + " : " + value + "</pre></div>");
}

}  // namespace

namespace sequencergui
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
    if (IsPropertyToShow(property->GetTagIndex().tag))
    {
      AppendNameValuePair(property->GetDisplayName(), mvvm::utils::ValueToString(property->Data()),
                          text_edit);
    }
  }

  return text_edit.toHtml();
}

}  // namespace sequencergui
