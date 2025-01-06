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

#include "xml_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sequencergui/transform/procedure_item_transform_helper.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/sequence_parser.h>

#include <stdexcept>

namespace
{

/**
 * @brief Finds position of two quotes in a string starting from given position.
 */
std::pair<size_t, size_t> FindQuotes(const std::string &str, size_t pos = 0)
{
  const char quote = '"';
  auto pos1 = str.find(quote, pos);
  size_t pos2 = pos1 == std::string::npos ? std::string::npos : str.find(quote, pos1 + 1);
  return {pos1, pos2};
}

/**
 * @brief Replaces all occurences of HTML-quot symbol with normal double quotes.
 *
 * @param str Reference to the string to be modified.
 *
 * @param pos1 The position to start search.
 * @param pos2 The position to end the search.
 *
 * @return New position of the end if replacement had occured.
 */
size_t ReplaceHtmlQuotes(std::string &str, size_t pos1, size_t pos2)
{
  const std::string double_quote("\"");
  const std::string html_quote = "&quot;";

  auto pos_html = str.find(html_quote, pos1);

  while (pos_html < pos2)
  {
    str.replace(pos_html, html_quote.length(), double_quote);
    pos_html = str.find(html_quote, pos_html + double_quote.length());
    pos2 = pos2 - html_quote.length() + double_quote.length();  // new end adjust
  }

  return pos2;
}

}  // namespace

namespace sequencergui
{

std::unique_ptr<ProcedureItem> ImportFromFile(const std::string &file_name)
{
  auto procedure = sup::sequencer::ParseProcedureFile(file_name);

  if (!procedure)
  {
    throw std::runtime_error("Error: uninitialised procedure");
  }

  auto result = CreateProcedureItem(*procedure);
  result->SetFileName(file_name);

  return result;
}

std::string ExportToXMLString(const ProcedureItem &procedure_item)
{
  DomainProcedureBuilder builder;
  auto domain_procedure = builder.CreateProcedure(procedure_item);
  return ReplaceQuotationMarks(GetXMLString(*domain_procedure));
}

std::string ReplaceQuotationMarks(const std::string &str)
{
  std::string result(str);

  auto pos = FindQuotes(result);

  while (pos.first != std::string::npos && pos.second != std::string::npos)
  {
    auto new_pos2 = ReplaceHtmlQuotes(result, pos.first + 1, pos.second);

    // if new end of the string differs, we have to turn external double quotes to single quotes.
    if (new_pos2 != pos.second)
    {
      const char single_quote = '\'';
      result[pos.first] = single_quote;
      result[new_pos2] = single_quote;
    }

    pos = FindQuotes(result, new_pos2 + 1);
  }

  return result;
}

}  // namespace sequencergui
