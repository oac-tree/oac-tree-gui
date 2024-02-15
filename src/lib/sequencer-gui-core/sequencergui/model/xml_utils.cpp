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

#include "xml_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sequencergui/transform/transform_from_domain.h>

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

  auto result = CreateProcedureItem(procedure.get(), /*root_only*/ false);
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
  const std::string double_quote("\"");
  const std::string html_quote = "&quot;";
  const char single_quote = '\'';

  std::string result(str);
  auto pos = FindQuotes(result);

  while (pos.first != std::string::npos && pos.second != std::string::npos)
  {
    auto pos_html = result.find(html_quote, pos.first + 1);

    bool contains = false;
    while (pos_html < pos.second)
    {
      result.replace(pos_html, html_quote.length(), double_quote);
      pos.second = pos.second - html_quote.length() + double_quote.length();

      pos_html = result.find(html_quote, pos_html + 1);
      contains = true;
    }
    if (contains)
    {
      result[pos.first] = single_quote;
      result[pos.second] = single_quote;
    }

    pos = FindQuotes(result, pos.second + 1);
  }

  return result;
}

}  // namespace sequencergui
